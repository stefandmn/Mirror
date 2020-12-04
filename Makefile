SHELL:=/bin/bash
ROOT=$(shell pwd)
NAME=$(shell basename $(ROOT))

ifndef VERBOSE
.SILENT:
endif

OUTDIR=
SYSDIR=sys
SRCDIR=src
DEBDIR=deb


ifeq ($(DEVICE),)
	export DEVICE=RPi
endif

ifeq ($(OUTPUT),)
	export OUTPUT=$(ROOT)/../Clue-out
endif


OUTDIR=$(OUTPUT)/$(NAME)
BUILDFS=$(OUTPUT)/devel-$(DEVICE).arm-Raspbian
DISTRO_VER=$(shell more $(DEBDIR)/control | grep "Version:" | cut -f2 -d":" | sed -e 's/^[ \t]*//')
DISTRO_REL=$(shell echo "$(DISTRO_VER)" | cut -f1 -d".")
DISTRO_MAJ=$(shell echo "$(DISTRO_VER)" | cut -f2 -d".")
DISTRO_MIN=$(shell echo "$(DISTRO_VER)" | cut -f3 -d".")
NEXT_MIN=$(shell python -c "print int($(DISTRO_MIN)) + 1")
NEXT_VER="${DISTRO_REL}.${DISTRO_MAJ}.${NEXT_MIN}"


info:
	echo -e "\tName:         $(NAME)"
	echo -e "\tVersion:      $(DISTRO_VER)"
	echo -e "\tPackage File: $(OUTPUT)/targets/$(NAME).tar.gz"


prereqs:
ifeq ($(shell sudo chroot $(BUILDFS)/root dpkg-query -W -f='${Status} ${Version}\n' libvncserver-dev 2>/dev/null | wc -l),0)
	sudo chroot $(BUILDFS)/root apt-get update
	sudo chroot $(BUILDFS)/root apt-get install libvncserver-dev libconfig++-dev -y
endif


setupenv:
ifeq ($(shell dpkg-query -W -f='${Status} ${Version}\n' qemu 2>/dev/null | wc -l),0)
	sudo apt-get install qemu
endif
ifeq ($(shell dpkg-query -W -f='${Status} ${Version}\n' kpartx 2>/dev/null | wc -l),0)
	sudo apt-get install kpartx
endif
ifneq ($(shell [[ -d $(BUILDFS) ]] && echo "yes"),yes)
	mkdir -p $(BUILDFS)/root
	mkdir -p $(BUILDFS)/boot
endif
ifneq ($(shell [[ -f $(BUILDFS)/raspbian.img ]] && echo "yes"),yes)
	wget http://downloads.raspberrypi.org/raspbian_latest -O /tmp/raspbian.zip
	unzip /tmp/raspbian.zip -d $(BUILDFS)/
	mv $(BUILDFS)/*.img $(BUILDFS)/raspbian.img
	rm -rf /tmp/raspbian.zip
endif
ifeq ($(shell sudo losetup -a | wc -l),0)
	sudo losetup -D
	sudo losetup -P /dev/loop0 $(BUILDFS)/raspbian.img
	sudo mount /dev/loop0p1 $(BUILDFS)/boot
	sudo mount /dev/loop0p2 $(BUILDFS)/root
endif
ifneq ($(shell [[ -f $(BUILDFS)/root/usr/bin/qemu-arm-static ]] && echo "yes"),yes)
	sudo cp -rf /usr/bin/qemu-arm-static $(BUILDFS)/root/usr/bin/
endif
ifeq ($(shell [[ -f $(BUILDFS)/root/etc/ld.so.preload ]] && echo "yes"),yes)
	sudo rm -rf $(BUILDFS)/root/etc/ld.so.preload
endif
ifeq ($(shell mount | grep "/root/proc" 2>/dev/null | wc -l),0)
	sudo mount -t proc proc $(BUILDFS)/root/proc
	sudo mount -t sysfs sysfs $(BUILDFS)/root/sys
	sudo mount -t devpts devpts $(BUILDFS)/root/dev/pts
endif
	$(MAKE) prereqs


cleanenv:
ifneq ($(shell mount | grep "/root/proc" 2>/dev/null | wc -l),0)
	sudo umount $(BUILDFS)/root/proc
	sudo umount $(BUILDFS)/root/sys
	sudo umount $(BUILDFS)/root/dev/pts
endif
ifneq ($(shell sudo losetup -a | wc -l),0)
	sudo umount $(BUILDFS)/boot
	sudo umount $(BUILDFS)/root
	sudo losetup -D
	sudo kpartx -d /dev/loop0
endif


# Mark new revision (addon version) in the development copy
version:
	sed -i "s|Version: $(DISTRO_VER)|Version: $(NEXT_VER)|g" $(DEBDIR)/control


# Build addon package in deployment format
build:
	$(MAKE) setupenv
	sudo rm -rf $(BUILDFS)/root/root/$(NAME)
	sudo cp -rf $(SRCDIR) $(BUILDFS)/root/root/$(NAME)
	sudo chroot $(BUILDFS)/root /bin/bash -c "cd /root/Mirror && make"
ifneq ($(shell [[ -d $(OUTDIR)/dist ]] && echo "yes"),yes)
	mkdir -p $(OUTDIR)/dist/usr/bin
	mkdir -p $(OUTDIR)/dist/etc
	mkdir -p $(OUTDIR)/dist/lib
	cp -rf $(ROOT)/$(SYSDIR)/etc/* $(OUTDIR)/dist/etc/
	cp -rf $(ROOT)/$(SYSDIR)/lib/* $(OUTDIR)/dist/lib/
endif
	sudo cp -rf $(BUILDFS)/root/root/$(NAME)/mirror $(OUTDIR)/dist/usr/bin/mirror
	sudo chown $(USER) $(OUTDIR)/dist/usr/bin/mirror


distro: build
	mkdir -p ${OUTDIR}/dist/DEBIAN
	sudo cp -rf ${ROOT}/$(DEBDIR)/* ${OUTDIR}/dist/DEBIAN/
	sudo chmod 755 ${OUTDIR}/dist/DEBIAN/*
	sudo sed -i "s|Installed-Size: 0000|Installed-Size: $(shell du -s $(OUTDIR)/dist | cut -f 1)|g" $(OUTDIR)/dist/DEBIAN/control
	sudo chown -R root:root $(OUTDIR)/dist
	/usr/bin/dpkg-deb -z8 -Zgzip --build $(OUTDIR)/dist $(OUTPUT)/targets/$(NAME).deb >/dev/null
	cd ${OUTDIR}/dist && /bin/tar -zcvf $(OUTPUT)/targets/$(NAME).tar.gz usr etc lib >/dev/null


# Commit and push updated files into versioning system (GitHUB). The 'message' input
# parameter is required.
gitrev:
ifneq ($(message),)
	git add .
	git commit -m "$(message)"
	git push
else
	@printf "\n* Please specify 'message' parameter!\n\n"
	exit 1
endif


# Create and push a new versioning tag equals with the addon release. The uploaded can be
# done later - manually or through a separate task and thus the tag is transformed into a
# addon release
gitrel:
	git tag "$(DISTRO_VER)"
	git push origin --tags


# Combine git commit and git release tasks into a single one, the only exception is that
# the commit doesn't require a message, if the message exist it will be used, if not a
# standard commit message will be composed using the addon version number
git:
ifeq ($(message),)
	$(MAKE) gitrev -e message="Release $(DISTRO_VER)"
else
	$(MAKE) gitrev
endif
	$(MAKE) gitrel


# Create a complete release: new build, publish it in the repository, update the versioning
release:
	$(MAKE) version
	$(MAKE) distro
	$(MAKE) git
	$(MAKE) clean


# Clean-up the release
clean:
ifneq ($(shell sudo losetup -a | wc -l),0)
	sudo rm -rf $(BUILDFS)/root/root/$(NAME)
endif
	$(MAKE) cleanenv
	sudo rm -rf $(OUTDIR)


# Clean-up all build distributions, cache and stamps
cleanall:
	sudo rm -rf $(OUTDIR)/* $(OUTDIR)/.stamp $(OUTDIR)/.ccache


# Display the help text
help:
	echo -e "\
\nSYNOPSIS\n\
       make info \n\
       version | build | release \n\
       make gitrev | gittag | version \n\
       make clean | cleanall \n\
       make help \n\
\nDESCRIPTION\n\
    Executes one of the make targets defined through this Makefile flow, according \n\
    to the scope of this project.\n\n\
    info\n\
                  provides main details about current release: package name, version id\n\
                  and package file (should be found after execution of 'build' target)\n\
                  >> this is the default target wihin the CCM process\n\
    version\n\
                  create local new version within local addon descriptor (addon.xml),\n\
                  the new version being the incremented value fo previous version\n\
                  (for the minor version number)\n\
    build\n\
                  build the addon package along to the new version and prepare the release\n\
                  package file within location $(OUTPUT)/targets/$(NAME).tar.gz\n\
    release\n\
                  Build the addon providing new local version, make release and publish it\n\
                  on the Clue repository (already mounted to the local file system). Then\n\
                  the released version is submitted in the versioning system (GitHUB) over a\n\
                  new release tag version\n\
    gitrev\n\
                  Commit the new release changes into versioning repository (GitHub)\n\
    gitrel\n\
                  Create a new release tag into versioning repository (GitHub) using current\n\
                  addon version (defined in the addon descriptor - addon.xml file)\n\
    git\n\
                  Combine git commit and git release targets into a single one, the only exception\n\
                  is that the commit doesn't require a message description; in case the message exist\n\
                  it will be used as such, otherwise a standard commit message will be composed using\n\
                  the addon version number\n\
    clean\n\
                  cleanup the build resources within the output location\n\
    cleanall\n\
                  Clean-up all resources from the output location (related or nor directly\n\
                  connected to the addon build process\n\
    help\n\
                  Shows this text\n\
\n\
EXAMPLES\n\
       build the entire distribution ('build' make task is default)\n\
       > make build\n\n\
       make complete addon release: build it, publish it and commit the changes on GitHub\n\
       > make release\n\n\
" | more

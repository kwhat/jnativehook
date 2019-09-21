FROM gentoo/stage3-amd64:latest

WORKDIR /root

# Install portage from the latest snapshot
RUN curl http://www.gtlib.gatech.edu/pub/gentoo/snapshots/portage-latest.tar.bz2 | tar -xj -C /usr

# Setup the container locale
COPY docker/locale.gen /etc/locale.gen
RUN locale-gen

# Setup native x86_64 system
COPY docker/x86_64-pc-linux-gnu.make.conf /etc/portage/make.conf
COPY docker/package.use /etc/portage/package.use/x86_64-pc-linux-gnu
COPY docker/package.provided /etc/portage/profile/package.provided
RUN echo "sys-devel/crossdev::crossdev-apple-overlay" > /etc/portage/package.unmask
RUN mkdir -p /etc/portage/package.keywords
RUN echo "=sys-devel/crossdev-99999999 **" > /etc/portage/package.keywords/x86_64-pc-linux-gnu

RUN mkdir -p /usr/local/portage/metadata
RUN echo "masters = gentoo" > /usr/local/portage/metadata/layout.conf

RUN mkdir -p /usr/local/portage/profiles
RUN echo "cross_overlay" > /usr/local/portage/profiles/repo_name
RUN eselect profile set default/linux/amd64/17.1

# Create the Gentoo repo via webrsync
RUN emerge-webrsync
RUN eselect news read --quiet

# Update the stage3 gcc, binutils and portage before installing other dependencies
RUN emerge -1vuDN world

# Setup the new gcc and binutils
RUN gcc-config x86_64-pc-linux-gnu-9.2.0
RUN binutils-config --linker ld.gold
RUN . /etc/profile

# We need to install git so that we can sync our custom apple overlay
RUN emerge -vu dev-vcs/git
COPY docker/crossdev-apple-overlay.conf /etc/portage/repos.conf/crossdev-apple-overlay.conf

# Do a full system update
RUN emerge --sync
RUN eselect news read --quiet
RUN emerge -vuDN world

# Installl some Gentoo tools
RUN emerge -vu gentoolkit
RUN emerge -vu crossdev

# Install Java via IcedTea
RUN emerge -vu icedtea
RUN emerge -vu ant
RUN eselect java-vm set system icedtea-8

# Get the Windows JNI headers from AdoptOpenJDK
RUN curl -c /tmp/cookies.txt -b /tmp/cookies.txt -L "https://github.com/AdoptOpenJDK/openjdk8-binaries/releases/download/jdk8u222-b10/OpenJDK8U-jdk_x86-32_windows_hotspot_8u222b10.zip" -o /tmp/OpenJDK8U-jdk_x86-32_windows_hotspot_8u222b10.zip
RUN unzip /tmp/OpenJDK8U-jdk_x86-32_windows_hotspot_8u222b10.zip 'jdk*/include/*' -d /tmp/
RUN mv /tmp/jdk*/include /opt/windows-jdk-1.8

# Install x86_64 deps for libUIOHook
RUN emerge -vu xcb-proto
RUN emerge -vu libXt
RUN emerge -vu libXtst
RUN emerge -vu libXinerama
RUN emerge -vu libxkbcommon
RUN emerge -vu libxkbfile
RUN emerge -vu libxcb

# Setup i686-pc-linux-gnu cross compiler
#COPY docker/i686-pc-linux-gnu.make.conf /usr/i686-cc-linux-gnu/etc/portage/make.conf
#COPY docker/package.use /etc/portage/package.use/cross-i686-cc-linux-gnu
#RUN crossdev -t i686-cc-linux-gnu -P -v
#RUN ln -s /usr/portage/profiles/default/linux/x86/17.0 /usr/i686-cc-linux-gnu/etc/portage/make.profile
#RUN i686-cc-linux-gnu-emerge --sync
#RUN i686-cc-linux-gnu-emerge -uv xcb-proto
#RUN i686-cc-linux-gnu-emerge -uv libXt
#RUN i686-cc-linux-gnu-emerge -uv libXtst
#RUN i686-cc-linux-gnu-emerge -uv libXinerama
#RUN i686-cc-linux-gnu-emerge -uv libxkbcommon
#RUN i686-cc-linux-gnu-emerge -uv libxkbfile 
#RUN i686-cc-linux-gnu-emerge -uv libxcb
#RUN gcc-config i686-cc-linux-gnu-9.2.0

# Setup armv6j-hardfloat-linux-gnueabihf cross compiler
COPY docker/armv6j-hardfloat-linux-gnueabihf.make.conf /usr/armv6j-hardfloat-linux-gnueabihf/etc/portage/make.conf
COPY docker/package.use /etc/portage/package.use/cross-armv6j-hardfloat-linux-gnueabihf
RUN crossdev -t armv6j-hardfloat-linux-gnueabihf -P -v
RUN ln -s /usr/portage/profiles/default/linux/arm/17.0 /usr/armv6j-hardfloat-linux-gnueabihf/etc/portage/make.profile
RUN gcc-config armv6j-hardfloat-linux-gnueabihf-9.2.0
#RUN armv6j-hardfloat-linux-gnueabihf-emerge --sync

# Install armv6j deps for libUIOHook
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu xcb-proto
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libXt
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libXtst
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libXinerama
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libxkbcommon
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libxkbfile
RUN armv6j-hardfloat-linux-gnueabihf-emerge -vu libxcb

# Setup i686-w64-mingw32 cross compiler
RUN crossdev -t i686-w64-mingw32 -P -v

# Setup x86_64-w64-mingw32 cross compiler
RUN crossdev -t x86_64-w64-mingw32 -P -v

# Setup i686-apple-darwin cross compiler
RUN crossdev --stage0 --binutils 4.6 --target i686-apple-darwin -P -v

# Setup x86_64-apple-darwin cross compiler
RUN crossdev --stage0 --binutils 4.6 --target x86_64-apple-darwin -P -v

# Install the Apple SDK 10.6
RUN emerge -vu sys-darwin/apple-sdk:10.6

# Setup enviroment variables
ENV PATH /usr/lib/llvm/9/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/opt/bin

# System clean up
RUN emerge --depclean
RUN eclean-dist -d
RUN rm -R /usr/portage/distfiles/* /tmp/* /var/tmp/*

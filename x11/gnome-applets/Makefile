# Created by: Joe Marcus Clarke <marcus@FreeBSD.org>
# $FreeBSD$

PORTNAME=	gnome-applets
PORTVERSION=	3.17.1
CATEGORIES=	x11 gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Applets components for the Gnome Desktop Environment

BUILD_DEPENDS=	itstool:${PORTSDIR}/textproc/itstool \
		adwaita-icon-theme>=0:${PORTSDIR}/x11-themes/adwaita-icon-theme
LIB_DEPENDS=	libgtop-2.0.so:${PORTSDIR}/devel/libgtop \
		libxklavier.so:${PORTSDIR}/x11/libxklavier \
		libdbus-1.so:${PORTSDIR}/devel/dbus \
		libdbus-glib-1.so:${PORTSDIR}/devel/dbus-glib \
		libpanel-applet.so:${PORTSDIR}/x11/gnome-panel \
		libpolkit-gobject-1.so:${PORTSDIR}/sysutils/polkit \
		libhal.so:${PORTSDIR}/sysutils/hal \
		libupower-glib.so:${PORTSDIR}/sysutils/upower \
		libnotify.so:${PORTSDIR}/devel/libnotify
RUN_DEPENDS=	gucharmap:${PORTSDIR}/deskutils/gucharmap \
		adwaita-icon-theme>=0:${PORTSDIR}/x11-themes/adwaita-icon-theme

PORTSCOUT=	limitw:1,even

USES=		autoreconf:build gettext gmake libtool pathfix pkgconfig python:3 \
		shebangfix tar:xz
USE_XORG=	x11
USE_GNOME=	gdkpixbuf2 gtksourceview2 introspection:build \
		libxml2 libxslt pygobject pygobject3 libwnck intlhack
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
INSTALLS_ICONS=	yes
CONFIGURE_ARGS=	--disable-networkmanager \
		--disable-battstat \
		--disable-cpufreq
CONFIGURE_ENV=	OS_SYS="/usr/include"
CFLAGS+=	-I${LOCALBASE}/include -Wno-error=format-nonliteral
LDFLAGS+=	-lpthread -L${LOCALBASE}/lib
SHEBANG_FILES=	invest-applet/invest/invest-chart \
		invest-applet/invest/invest-applet.py
PLIST_SUB+=	PYVER=${PYTHON_VER:S/.//}
INSTALL_TARGET=	install-strip

GLIB_SCHEMAS=	org.gnome.gnome-applets.charpick.gschema.xml \
		org.gnome.gnome-applets.geyes.gschema.xml \
		org.gnome.gnome-applets.gweather.gschema.xml \
		org.gnome.gnome-applets.multiload.gschema.xml \
		org.gnome.gnome-applets.netspeed.gschema.xml \
		org.gnome.gnome-applets.stickynotes.gschema.xml \
		org.gnome.gnome-applets.window-picker-applet.gschema.xml

.include <bsd.port.mk>

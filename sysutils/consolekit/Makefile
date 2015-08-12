# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	consolekit
PORTVERSION=	1.0.0
CATEGORIES=	sysutils gnome

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Framework for defining and tracking users

LIB_DEPENDS=	libdbus-glib-1.so:${PORTSDIR}/devel/dbus-glib \
    		libpolkit-gobject-1.so:${PORTSDIR}/sysutils/polkit

USE_GITHUB=	yes
GH_ACCOUNT=	ConsoleKit2
GH_PROJECT=	ConsoleKit2

USES=		autoreconf gettext gmake libtool pathfix pkgconfig tar:bzip2
USE_XORG=	x11
GNU_CONFIGURE=	yes
USE_GNOME=	glib20
USE_LDCONFIG=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
CONFIGURE_ARGS=	--with-pid-file=/var/run/${PORTNAME}.pid \
		--enable-pam-module \
		--with-pam-module-dir=${PREFIX}/lib \
		--localstatedir=/var \
		--enable-polkit \
		--disable-udev-acl \
		--without-systemsystemunitdir
PATHFIX_MAKEFILEIN=	Makefile.am

INSTALL_TARGET=	install-strip

post-install:
	@${MKDIR} ${STAGEDIR}/var/log/ConsoleKit

.include <bsd.port.mk>

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
GH_TAGNAME=	fb6de51

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

post-patch:
	@${REINPLACE_CMD} -e 's|/usr/bin/|${PREFIX}/bin/|g' \
		${WRKSRC}/data/90-consolekit

post-install:
	@${MKDIR} ${STAGEDIR}/var/log/ConsoleKit
	@${MKDIR} ${STAGEDIR}${PREFIX}/etc/X11/xinit/xinitrc.d
	${INSTALL_DATA} ${WRKSRC}/data/90-consolekit \
		${STAGEDIR}${PREFIX}/etc/X11/xinit/xinitrc.d

.include <bsd.port.mk>

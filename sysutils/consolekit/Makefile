# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	consolekit
PORTVERSION=	1.0.1
CATEGORIES=	sysutils gnome
MASTER_SITES=	https://github.com/ConsoleKit2/ConsoleKit2/releases/download/${PORTVERSION}/
DISTFILES=	ConsoleKit2-${PORTVERSION}${EXTRACT_SUFX}

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Framework for defining and tracking users

LICENSE=	GPLv2

LIB_DEPENDS=	libdbus-1.so:devel/dbus \
    		libpolkit-gobject-1.so:sysutils/polkit

WRKSRC=		${WRKDIR}/ConsoleKit2-${PORTVERSION}
USES=		gettext gmake gnome libtool pathfix pkgconfig tar:bzip2
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
INSTALL_TARGET=	install-strip

post-install:
	@${MKDIR} ${STAGEDIR}/var/log/ConsoleKit

.include <bsd.port.mk>

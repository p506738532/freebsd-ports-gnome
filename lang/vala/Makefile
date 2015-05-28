# Created by: Michael Johnson <ahze@FreeBSD.org>
# $FreeBSD$

PORTNAME=	vala
PORTVERSION=	0.29.1
CATEGORIES=	lang gnome
MASTER_SITES=	GNOME

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Programming language and compiler that converts Vala code into C code

LICENSE=	LGPL21

USES=		bison gettext gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	glib20 gnomeprefix libxslt
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
USE_LDCONFIG=	yes
INSTALL_TARGET=	install-strip

VALA_VER=	${PORTVERSION:R}

# Version got bumped to 0.30, replace VERSION with that instead
# of using VALA_VER. Replace 0.30 with VALA_VER when vala gets to 0.30.0
PLIST_SUB=	VERSION=0.30

post-install:
	${LN} -fs ${PREFIX}/libdata/pkgconfig/libvala-${VALA_VER}.pc \
		${STAGEDIR}${PREFIX}/libdata/pkgconfig/vala-1.0.pc

.include <bsd.port.mk>

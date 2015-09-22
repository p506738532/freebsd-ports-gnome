# Created by: Koop Mast <kwm@FreeBSD.org>
# $FreeBSD$

PORTNAME=	atkmm
PORTVERSION=	2.24.1
CATEGORIES=	accessibility gnome
MASTER_SITES=	GNOME

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	C++ wrapper for ATK API library

PORTSCOUT=	limitw:1,even

USES=		compiler:c++11-lib gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	atk glibmm
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip
CONFIGURE_ARGS=	--disable-documentation

.include <bsd.port.mk>

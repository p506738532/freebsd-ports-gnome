# Created by: Janni
# $FreeBSD$

PORTNAME=	eris
PORTVERSION=	1.3.23
PORTREVISION=	5
CATEGORIES=	devel
MASTER_SITES=	SF/worldforge/Eris%201.4%20%28client%20lib%29/${PORTVERSION}/

MAINTAINER=	oliver@FreeBSD.org
COMMENT=	WorldForge client entity library

LIB_DEPENDS=	libAtlas-0.6.so:devel/atlas-devel \
		libskstream-0.3.so:net/skstream \
		libwfmath-1.0.so:math/wfmath \
		libsigc-2.0.so:devel/libsigc++20 \
		libmercator-0.3.so:devel/mercator

CONFIGURE_ARGS= --disable-silent-rules
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
USES=		libtool pathfix pkgconfig compiler:c++11-lib

.include <bsd.port.mk>

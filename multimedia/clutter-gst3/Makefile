# Created by: Martin Wilke <miwi@FreeBSD.org>
# $FreeBSD$

PORTNAME=	clutter-gst
PORTVERSION=	3.0.0
CATEGORIES=	multimedia
MASTER_SITES=	GNOME
PKGNAMESUFFIX=	3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Clutter GStreamer integration

LIB_DEPENDS=	libclutter-1.0.so:${PORTSDIR}/graphics/clutter

USES=		gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	gnomeprefix introspection:build
GNU_CONFIGURE=	yes
USE_GL=		gl
USE_GSTREAMER1=	yes
USE_XORG=	xp x11
USE_LDCONFIG=	yes
MAKE_ENV=	XDG_CACHE_HOME=${WRKDIR}

CPPFLAGS+=	-I${LOCALBASE}/include ${PTHREAD_CFLAGS}
LDFLAGS+=	-L${LOCALBASE}/lib ${PTHREAD_LIBS}
INSTALL_TARGET=	install-strip

.include <bsd.port.mk>

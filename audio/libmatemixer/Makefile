# $FreeBSD

PORTNAME=	libmatemixer
PORTVERSION=	1.9.1
CATEGORIES=	audio mate
MASTER_SITES=	MATE
DIST_SUBDIR=	mate

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Mixer library for MATE desktop

LICENSE=	LGPL21

USES=	libtool pathfix pkgconfig tar:xz
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--disable-alsa \
		--disable-pulseaudio \
		--disable-null \
		--enable-oss \
		--disable-gtk-doc \
		--disable-gtk-doc-html \
		--disable-gtk-doc-pdf
INSTALL_TARGET=	install-strip

.include <bsd.port.mk>

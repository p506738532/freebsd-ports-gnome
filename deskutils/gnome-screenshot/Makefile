# Created by: Joe Marcus Clarke <marcus@FreeBSD.org>
# $FreeBSD$

PORTNAME=	gnome-screenshot
PORTVERSION=	3.20.1
CATEGORIES=	deskutils gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	GNOME 3 utility for making picutures of your screen

LIB_DEPENDS=	libcanberra-gtk3.so:audio/libcanberra-gtk3

CONFLICTS=	gnome-utils-2.[0-9]*
PORTSCOUT=	limitw:1,even

USES=		gettext gmake gnome localbase pathfix pkgconfig tar:xz
USE_GNOME=	gtk30 intlhack
GNU_CONFIGURE=	yes

GLIB_SCHEMAS=	org.gnome.gnome-screenshot.gschema.xml

.include <bsd.port.mk>

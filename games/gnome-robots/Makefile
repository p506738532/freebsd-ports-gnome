# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	gnome-robots
PORTVERSION=	3.20.1
CATEGORIES=	games gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Gnome robots

BUILD_DEPENDS=	itstool:textproc/itstool
LIB_DEPENDS=	libcanberra-gtk3.so:audio/libcanberra-gtk3 \
		libgames-support.so:games/libgames-support

PORTSCOUT=	limitw:1,even

USES=		gettext gmake gnome localbase pathfix \
		pkgconfig tar:xz
USE_GNOME=	gtk30 intlhack librsvg2 libxml2:build
INSTALLS_ICONS=	yes
GNU_CONFIGURE=	yes
CFLAGS=		-I/usr/local/include/gnome-games/
CONFIGURE_ARGS=	--localstatedir=${PREFIX}/share

GLIB_SCHEMAS=	org.gnome.robots.gschema.xml

.include <bsd.port.mk>

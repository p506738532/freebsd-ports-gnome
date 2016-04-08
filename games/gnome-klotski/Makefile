# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	gnome-klotski
PORTVERSION=	3.20.0
CATEGORIES=	games gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Gnome klotski

BUILD_DEPENDS=	vala:lang/vala \
		itstool:textproc/itstool
LIB_DEPENDS=	libgames-support.so:games/libgames-support \

PORTSCOUT=	limitw:1,even

USES=		gettext gmake gnome localbase pathfix \
		pkgconfig tar:xz
USE_GNOME=	gtk30 intlhack librsvg2 libxml2:build
INSTALLS_ICONS=	yes
GNU_CONFIGURE=	yes

GLIB_SCHEMAS=	org.gnome.klotski.gschema.xml

.include <bsd.port.mk>

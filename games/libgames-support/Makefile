# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	libgames-support
PORTVERSION=	0.0.${SNAPDATE}
CATEGORIES=	games gnome
#MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Shared code between gnome games

BUILD_DEPENDS=	valac:${PORTSDIR}/lang/vala
LIB_DEPENDS=	libgee-0.8.so:${PORTSDIR}/devel/libgee

PORTSCOUT=	limitw:1,even

# until we can have a propper release
USE_GITHUB=	yes
GH_ACCOUNT=	GNOME
GH_PROJECT=	libgames-support
GH_TAGNAME=	222d43de3c
SNAPDATE=	20150819
PATHFIX_MAKEFILEIN?=	Makefile.am

USES=		autoreconf gettext gmake libtool pathfix pkgconfig
USE_GNOME=	gtk30 intltool
GNU_CONFIGURE=	yes

.include <bsd.port.mk>

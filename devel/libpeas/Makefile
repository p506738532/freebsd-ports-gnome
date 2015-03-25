# Created by: Koop Mast <kwm@FreeBSD.org>
# $FreeBSD$

PORTNAME=	libpeas
PORTVERSION=	1.14.0
CATEGORIES=	devel gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	kwm@FreeBSD.org
COMMENT=	Next evolution of the Gedit plugins engine

BUILD_DEPENDS=	valac:${PORTSDIR}/lang/vala

PORTSCOUT=	limitw:1,even

USES=		gettext gmake libtool pathfix pkgconfig python:3 tar:xz
USE_GNOME=	gtk30 intlhack introspection:build
USE_LDCONFIG=	yes
INSTALLS_ICONS=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LIBS+=		-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip
CONFIGURE_ARGS+=--disable-glade-catalog \
		--disable-python2

OPTIONS_SUB=	yes
OPTIONS_DEFINE=	PYTHON
OPTIONS_DEFAULT=PYTHON

# there is support for python2 but can't figure out a how to include that
# in a sane way together with python3.
PYTHON_USE=	GNOME=py3gobject3
PYTHON_CONFIGURE_ENABLE=	python3

# lua needs lua 5.1 and devel/lua-lgi. The latter is build for lua 5.2 however..
CONFIGURE_ARGS+=	--disable-lua5.1 --disable-luajit

PLIST_SUB+=	PYVER=${PYTHON_VER:S/.//}

.include <bsd.port.mk>

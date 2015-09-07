# Created by: Martin Klaffenboeck <martin.klaffenboeck@gmx.at>
# $FreeBSD$

PORTNAME=	gtkmm
PORTVERSION=	2.24.4
PORTREVISION?=	3
CATEGORIES=	x11-toolkits
#MASTER_SITES=	GNOME
PKGNAMESUFFIX=	24
DIST_SUBDIR=	gnome2

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	C++ wrapper for Gtk+

#USES+=		tar:xz

PORTSCOUT=	ignore:1

# temp until new release is out which uses the new glibmm c++11 stuff
BUILD_DEPENDS+=	mm-common>=0:${PORTSDIR}/devel/mm-common
USE_GITHUB=	yes
GH_ACCOUNT=	GNOME
1GH_PROJECT=	gtkmm
GH_TAGNAME=	33f4ecd
USES=		autoreconf:build

.if !defined(REFERENCE_PORT)

BUILD_DEPENDS+=	gm4:${PORTSDIR}/devel/m4

GNU_CONFIGURE=	yes
USES+=		compiler:c++11-lib gettext gmake libtool pathfix pkgconfig
USE_LDCONFIG=	yes
USE_GNOME=	atkmm gtk20 intltool pangomm
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
CONFIGURE_ARGS=	--disable-documentation

PATHFIX_MAKEFILEIN=	Makefile.am

PLIST_SUB=	VERSION="2.4"

post-patch:
	@${REINPLACE_CMD} -e '/^SUBDIRS =/s/tests//' \
		${WRKSRC}/Makefile.am

pre-configure:
	cd ${WRKSRC} && ${SETENV} NOCONFIGURE=yes ${SH} ./autogen.sh

pre-build:
	cd ${WRKSRC}/gdk/src && ${GMAKE} ${_MAKE_JOBS}
	cd ${WRKSRC}/gtk/src && ${GMAKE} ${_MAKE_JOBS}
# something goes wrong during regen of the files, apply hack to fix this.
	@${REINPLACE_CMD} -e '/std::move(src)/d' \
		${WRKSRC}/gtk/gtkmm/*.cc

.include <bsd.port.mk>

.endif

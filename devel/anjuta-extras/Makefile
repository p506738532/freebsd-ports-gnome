# Created by: Koop Mast <kwm@FreeBSD.org>
# $FreeBSD$

PORTNAME=	anjuta-extras
PORTVERSION=	3.10.0
CATEGORIES=	devel
MASTER_SITES=	GNOME

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Extra anjuta plugins

LIB_DEPENDS=	libanjuta-3.so:${PORTSDIR}/devel/anjuta
		
USES=		gettext gmake libtool tar:xz
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
INSTALL_TARGET=	install-strip
USE_GNOME=	gtk30 intltool
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
CONFIGURE_ARGS+=--disable-plugin-valgrind \
		--localedir=${PREFIX}/share

GLIB_SCHEMAS=	org.gnome.anjuta.plugins.scintilla.gschema.xml \
		org.gnome.anjuta.plugins.scratchbox.gschema.xml

.include <bsd.port.mk>

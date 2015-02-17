# Created by: Adam Weinberger <adamw@FreeBSD.org>
# $FreeBSD$

PORTNAME=	devhelp
PORTVERSION=	3.15.90
CATEGORIES=	devel gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Developer help browser for GNOME 3

LIB_DEPENDS=	libwebkitgtk-3.0.so:${PORTSDIR}/www/webkit-gtk3

USES=		gettext gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	gtk30 intlhack libxslt
USE_XORG=	x11
GNU_CONFIGURE=	yes
INSTALLS_ICONS=	yes
USE_LDCONFIG=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip

GLIB_SCHEMAS=	org.gnome.devhelp.gschema.xml

post-patch:
	@${REINPLACE_CMD} -e 's|gedit-plugin||g' ${WRKSRC}/misc/Makefile.in

.include <bsd.port.mk>

# Created by: Gustau Perez i Querol <gustau.perez@gmail.com>
# $FreeBSD$

PORTNAME=	gnome-shell-extension-weather
PORTVERSION=	0.0.0.20141126
CATEGORIES=	deskutils gnome
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Weather extension for the GNOME3 shell

RUN_DEPENDS=	gnome-shell>=0:${PORTSDIR}/x11/gnome-shell

USES=		autoreconf gettext gmake libtool pkgconfig tar:xz
USE_GNOME=	glib20 intlhack
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

USE_GITHUB=	yes
GH_ACCOUNT=	Neroth
GH_TAGNAME=	329139f

PKGMESSAGE=	../gnome-shell-extra-extensions/pkg-message.slaves

GLIB_SCHEMAS=	org.gnome.shell.extensions.weather.gschema.xml

post-patch:
	@${REINPLACE_CMD} -e 's|"3.14"|"3.14", "3.16"|g' \
		${WRKSRC}/data/metadata.json.in

.include <bsd.port.mk>

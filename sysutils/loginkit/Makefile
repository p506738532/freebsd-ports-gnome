# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	loginkit
PORTVERSION=	0.0.0.20150418
CATEGORIES=	sysutils gnome

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Stand alone logind shim

RUN_DEPENDS=	consolekit>=0.9.2:${PORTSDIR}/sysutils/consolekit

USE_GITHUB=	yes
GH_ACCOUNT=	dimkr
GH_PROJECT=	LoginKit
GH_TAGNAME=	f363547

NO_CONFIGURE=	yes

USES=		gmake gnome pkgconfig
USE_GNOME=	glib20

# work around until unstall script does this.
pre-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}/share/dbus-1/system-services

.include <bsd.port.mk>

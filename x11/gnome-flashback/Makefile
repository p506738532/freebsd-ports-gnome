# Created by: Gustau Pérez Querol <gustau.perez@gmail.com>
# $FreeBSD$
#   $MCom$

PORTNAME=	gnome-flashback
PORTVERSION=	3.17.1
CATEGORIES=	x11 gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Unofficial session and helper application

LIB_DEPENDS=	libpulse.so.0:${PORTSDIR}/audio/pulseaudio \
		libcanberra-gtk3.so:${PORTSDIR}/audio/libcanberra-gtk3 \
		libupower-glib.so:${PORTSDIR}/sysutils/upower
RUN_DEPENDS=	gnome-panel>=0:${PORTSDIR}/x11/gnome-panel \
		gnome-keyring>=3.0.0:${PORTSDIR}/security/gnome-keyring \
		gnome-power-manager>=3.0.0:${PORTSDIR}/sysutils/gnome-power-manager \
		nautilus>=3.0.0:${PORTSDIR}/x11-fm/nautilus \
		notification-daemon>=0:${PORTSDIR}/deskutils/notification-daemon \
		gnome-screensaver>=0:${PORTSDIR}/x11/gnome-screensaver \
		polkit-gnome>=0.105:${PORTSDIR}/sysutils/polkit-gnome

USES=		gettext gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	gnomedesktop3 gnomeprefix gtk30 intltool introspection \
		metacity
		
USE_GSTREAMER1=	yes
GNU_CONFIGURE=	yes
USE_LDCONFIG=	yes
CPPFLAGS+=	-I${LOCALASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

GLIB_SCHEMAS=	org.gnome.gnome-flashback.gschema.xml

post-install:
	${RM} ${STAGEDIR}${PREFIX}/libexec/gnome-flashback-compiz
	${RM} ${STAGEDIR}${PREFIX}/share/gnome-session/sessions/gnome-flashback-compiz.session
	${RM} ${STAGEDIR}${PREFIX}/share/xsessions/gnome-flashback-compiz.desktop

.include <bsd.port.mk>

# Created by: Gustau Perez i Querol <gustau.perez@gmail.com>
# $FreeBSD$

PORTNAME=	muffin
PORTVERSION=	2.6.1
CATEGORIES=	x11-wm gnome
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Compositing window manager forked from Mutter for use with Cinnamon

BUILD_DEPENDS=	zenity>=3.0.0:${PORTSDIR}/x11/zenity \
		gsettings-desktop-schemas>=3.4.0:${PORTSDIR}/devel/gsettings-desktop-schemas \
		gtkdoc-check:${PORTSDIR}/textproc/gtk-doc
LIB_DEPENDS=	libstartup-notification-1.so:${PORTSDIR}/x11/startup-notification \
		libclutter-glx-1.0.so:${PORTSDIR}/graphics/clutter \
		libcanberra-gtk3.so:${PORTSDIR}/audio/libcanberra-gtk3 \
		libcinnamon-desktop.so:${PORTSDIR}/x11/cinnamon-desktop
RUN_DEPENDS=	zenity>=3.0.0:${PORTSDIR}/x11/zenity

USE_GITHUB=	yes
GH_ACCOUNT=	linuxmint

USES=		autoreconf:build execinfo gettext libtool gmake \
		pathfix pkgconfig python:2
USE_GNOME=	gconf2 gnomedocutils gnomeprefix gtk30 \
		intlhack introspection:build
USE_XORG=	x11 xext xrandr xcomposite xcursor xinerama ice sm
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include -Wno-error=unused-function
CFLAGS+=	-I${LOCALBASE}/include -Wno-error=unused-function
LDFLAGS+=	-L${LOCALBASE}/lib
PATHFIX_MAKEFILEIN=	Makefile.am
CONFIGURE_ARGS=	--enable-compile-warnings=maximum
INSTALL_TARGET=	install-strip

GLIB_SCHEMAS=	org.cinnamon.muffin.gschema.xml

pre-configure:
		@cd ${WRKSRC} && ${SETENV} NOCONFIGURE=yes ${SH} autogen.sh

post-patch:
	@${REINPLACE_CMD} -e 's|REQUIRED_AUTOMAKE_VERSION=1.10|REQUIRED_AUTOMAKE_VERSION=1.14|g' \
		${WRKSRC}/autogen.sh

.include <bsd.port.mk>

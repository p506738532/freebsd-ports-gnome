# Created by: Joe Marcus Clarke <marcus@FreeBSD.org>
# $FreeBSD$

PORTNAME=	gnome-panel
PORTVERSION=	3.17.1
CATEGORIES=	x11 gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Panel component for the GNOME 3 Desktop

USES=		tar:xz

.if !defined(REFERENCE_PORT)

BUILD_DEPENDS=	itstool:${PORTSDIR}/textproc/itstool
LIB_DEPENDS=	libgweather-3.so:${PORTSDIR}/net/libgweather \
		libtelepathy-glib.so:${PORTSDIR}/net-im/telepathy-glib \
		libcanberra-gtk3.so:${PORTSDIR}/audio/libcanberra-gtk3 \
		libpolkit-gobject-1.so:${PORTSDIR}/sysutils/polkit
RUN_DEPENDS=	notification-daemon>=0:${PORTSDIR}/deskutils/notification-daemon

USES+=		gettext gmake libtool pathfix pkgconfig
USE_XORG=	ice sm x11 xau xrandr
USE_LDCONFIG=	yes
INSTALLS_ICONS=	yes
USE_GNOME=	dconf gconf2 gnomedesktop3 gnomemenus3 gnomeprefix \
		intltool librsvg2 libwnck3
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--disable-network-manager
CFLAGS+=	-I${LOCALBASE}/include -Wno-error=format-nonliteral
CPPFLAGS+=	-I${LOCALBASE}/include -Wno-error=format-nonliteral
LDFLAGS+=	-L${LOCALBASE}/lib
INSTALL_TARGET=	install-strip

GLIB_SCHEMAS=	org.gnome.gnome-panel.applet.clock.gschema.xml \
		org.gnome.gnome-panel.applet.fish.gschema.xml \
		org.gnome.gnome-panel.applet.window-list.gschema.xml \
		org.gnome.gnome-panel.applet.workspace-switcher.gschema.xml \
		org.gnome.gnome-panel.enums.xml \
		org.gnome.gnome-panel.gschema.xml \
		org.gnome.gnome-panel.launcher.gschema.xml \
		org.gnome.gnome-panel.menu-button.gschema.xml \
		org.gnome.gnome-panel.object.gschema.xml \
		org.gnome.gnome-panel.toplevel.gschema.xml

OPTIONS_DEFINE=	EVOLUTION
OPTIONS_DEFAULT=EVOLUTION
EVOLUTION_DESC=	Evolution calendar support
EVOLUTION_USE=	GNOME=evolutiondataserver3
EVOLUTION_CONFIGURE_ENABLE=	eds

post-patch:
	@${FIND} ${WRKSRC} -type f | \
		${XARGS} -n 10 ${REINPLACE_CMD} -e \
		's|"applications.menu"|"gnome-applications.menu"|'
	@${REINPLACE_CMD} -e 's|xsltproc -o| xsltproc --nonet -o|g' \
		${WRKSRC}/help/clock/Makefile.in \
		${WRKSRC}/help/fish/Makefile.in

post-install:
		${RM} ${STAGEDIR}${PREFIX}/share/xsessions/gnome-flashback-compiz.desktop

.include <bsd.port.mk>

.endif

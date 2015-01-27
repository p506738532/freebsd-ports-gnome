# Created by: FreeBSD GNOME Team <gnome@freebsd.org>
# $FreeBSD$

PORTNAME=	appstream-glib
PORTVERSION=	0.3.4
CATEGORIES=	devel
MASTER_SITES=	http://people.freedesktop.org/~hughsient/appstream-glib/releases/
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Library to help with AppStream metadata

BUILD_DEPENDS=	gtkdoc-check:${PORTSDIR}/textproc/gtk-doc
LIB_DEPENDS=	libsoup-2.4.so:${PORTSDIR}/devel/libsoup \
		libyaml.so:${PORTSDIR}/textproc/libyaml \
		libfontconfig.so:${PORTSDIR}/x11-fonts/fontconfig \
		libfreetype.so:${PORTSDIR}/print/freetype2

USES=		gettext gperf gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	gdkpixbuf2 glib20 gtk30 intlhack introspection:build \
		libxml2:build pango
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
USE_SQLITE=	3
CONFIGURE_ARGS=	--with-xml-catalog=${LOCALBASE}/share/xml/catalog \
		--disable-rpm \
		--disable-ostree
INSTALL_TARGET=	install-strip

OPTIONS_DEFINE=	MANPAGES
OPTIONS_DEFAULT=MANPAGES
OPTIONS_SUB=	yes
MANPAGES_CONFIGURE_ENABLE=	man
MANPAGE_BUILD_DEPENDS=	docbook-xml>=0:${PORTSDIR}/textproc/docbook-xml \
		docbook-xsl>=0:${PORTSDIR}/textproc/docbook-xsl
MANPAGES_USE=	GNOME=libxslt:build

.include <bsd.port.options.mk>

.if ${OSVERSION} < 1000000
LIB_DEPENDS+=	libarchive.so.13:${PORTSDIR}/archivers/libarchive
.else
CONFIGURE_ENV+=	LIBARCHIVE_CFLAGS=" " \
		LIBARCHIVE_LIBS="-larchive"
.endif

.include <bsd.port.mk>

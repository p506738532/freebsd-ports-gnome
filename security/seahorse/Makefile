# Created by: Dave McKay <dave@mu.org>
# $FreeBSD$

PORTNAME=	seahorse
PORTVERSION=	3.17.4
CATEGORIES=	security gnome
MASTER_SITES=	GNOME
DIST_SUBDIR=	gnome3

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	GNOME application for managing encryption keys (PGP, SSH)

# seahorse hardcodes the exact version (in 3.17.4) check 3.18 if this is still
# the case.
BUILD_DEPENDS=	gnupg=${GNUPG_VERSION}:${PORTSDIR}/security/gnupg \
		valac:${PORTSDIR}/lang/vala \
		itstool:${PORTSDIR}/textproc/itstool
LIB_DEPENDS=	libgpgme.so:${PORTSDIR}/security/gpgme \
		libgcr-3.so:${PORTSDIR}/security/gcr \
		libsecret-1.so:${PORTSDIR}/security/libsecret \
		libsoup-2.4.so:${PORTSDIR}/devel/libsoup
RUN_DEPENDS=	gnupg=${GNUPG_VERSION}:${PORTSDIR}/security/gnupg

PORTSCOUT=	limitw:1,even

USE_XORG=	sm
USE_LDCONFIG=	yes
USES=		gettext gmake libtool pathfix pkgconfig tar:xz
USE_GNOME=	intlhack gnomeprefix gtk30
GNU_CONFIGURE=	yes
INSTALLS_ICONS=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

GNUPG_VERSION=	2.1.8

GLIB_SCHEMAS=	org.gnome.seahorse.gschema.xml \
		org.gnome.seahorse.manager.gschema.xml \
		org.gnome.seahorse.window.gschema.xml

OPTIONS_SUB=	yes
OPTIONS_DEFINE=	LDAP AVAHI
OPTIONS_DEFAULT=LDAP AVAHI
LDAP_CONFIGURE_ENABLE=	ldap
LDAP_USE=	OPENLDAP=yes
LDAP_DESC=	LDAP keyserver support (for gpg)
AVAHI_DESC=	DNS-SD key sharing
AVAHI_CONFIGURE_ENABLE=	sharing
AVAHI_LIB_DEPENDS=	libavahi-client.so:${PORTSDIR}/net/avahi-app

post-patch:
	@${REINPLACE_CMD}  '/^GNUPG_ACCEPTED/s/2.1.4/${GNUPG_VERSION}/' \
		${WRKSRC}/configure

.include <bsd.port.mk>

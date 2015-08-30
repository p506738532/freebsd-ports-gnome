# Created by: Alexander Nedotsukov <bland@FreeBSD.org>
# $FreeBSD$

PORTNAME=	libsigc++
PORTVERSION=	2.5.3
CATEGORIES=	devel
MASTER_SITES=	GNOME

MAINTAINER=	gnome@FreeBSD.org
COMMENT=	Callback Framework for C++

BUILD_DEPENDS=	gm4:${PORTSDIR}/devel/m4

GNU_CONFIGURE=	yes
INSTALL_TARGET=	install-strip
USES=		compiler:c++11-lib gmake libtool pathfix perl5 tar:xz
USE_PERL5=	build
USE_LDCONFIG=	yes

PLIST_SUB=	API_VERSION="2.0"

OPTIONS_DEFINE=	DOCS

.include <bsd.port.options.mk>

.if ! ${PORT_OPTIONS:MDOCS}
CONFIGURE_ARGS+=--enable-documentation=no
.endif

post-patch:
	@${REINPLACE_CMD} -e "/^SUBDIRS =/s|tests||; \
		/^SUBDIRS =/s|examples||" ${WRKSRC}/Makefile.in

.include <bsd.port.mk>

# Created by: Steve Wills <swills@FreeBSD.org>
# $FreeBSD$

PORTNAME=	devise-async
PORTVERSION=	0.10.1
CATEGORIES=	devel rubygems
MASTER_SITES=	RG

MAINTAINER=	ruby@FreeBSD.org
COMMENT=	Send Devise emails in background

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

RUN_DEPENDS?=	rubygem-devise>=3.2:devel/rubygem-devise

SLAVEDIRS=	devel/rubygem-devise-async-rails4

BROKEN=		Not compatible with devel/rubygem-devise > 3
DEPRECATED=	Not compatible with devel/rubygem-devise > 3
EXPIRED_DATE=	2016-12-31

NO_ARCH=	yes
USE_RUBY=	yes
USES=		gem

.include <bsd.port.mk>

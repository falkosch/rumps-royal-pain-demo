FROM gcc:9.2.0

RUN apt-get update \
  && apt-get install -y --no-install-recommends unzip

ENV BUILD_WRAPPER_PACKAGE="build-wrapper-linux-x86"
ENV BUILD_WRAPPER_ZIP="${BUILD_WRAPPER_PACKAGE}.zip"
ENV BUILD_WRAPPER="build-wrapper-linux-x86-64"

ENV SONAR_QUBE_URL="https://sonarcloud.io/static/cpp/${BUILD_WRAPPER_ZIP}"

RUN mkdir -p ${BUILD_WRAPPER_PACKAGE} \
  && wget -q ${SONAR_QUBE_URL} \
  && unzip ${BUILD_WRAPPER_ZIP} \
  && rm -f ${BUILD_WRAPPER_ZIP} \
  && chmod +x ${BUILD_WRAPPER_PACKAGE}/${BUILD_WRAPPER} \
  && mv -t /usr/local ${BUILD_WRAPPER_PACKAGE}

RUN cd /usr/bin \
  && ln -sT /usr/local/${BUILD_WRAPPER_PACKAGE}/${BUILD_WRAPPER} ${BUILD_WRAPPER}

FROM quay.io/school21/alpine:3.16

RUN apk --no-cache --upgrade add \
    bash \
    valgrind \
    make \
    gcc \
    musl-dev

COPY . /s21_cat
WORKDIR "/s21_cat"
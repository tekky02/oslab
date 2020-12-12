FROM tekky03/alpine:musl as builder

WORKDIR /root/

ADD lab4 .

RUN c++ -std=c++17 -o main main.cc fileprinter.cc

FROM alpine:latest

WORKDIR /root/

RUN apk add --no-cache libstdc++

COPY --from=builder /root/main .

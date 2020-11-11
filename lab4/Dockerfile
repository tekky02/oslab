FROM tekky03/alpine:musl as builder

WORKDIR /root/

ADD lab4 .

RUN c++ -std=c++17 -o main main.cc fileprinter.cc

FROM alpine:latest

COPY --from=builder /root/* .

CMD ["./main"]
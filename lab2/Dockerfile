FROM tekky03/alpine:musl as builder

WORKDIR /root/

ADD lab2 .

RUN cc -o main main.c utils.c -lpthread

FROM alpine:latest

WORKDIR /root/

COPY --from=builder /root/main .

CMD ["./main"]
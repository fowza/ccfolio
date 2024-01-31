FROM ubuntu:23.10

RUN apt-get update && apt-get install -y libpistache-dev

COPY ./build/App/ccfolio-exe /app/ccfolio-exe

WORKDIR /app

CMD ["./ccfolio-exe"]

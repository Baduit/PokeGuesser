FROM alpine:3.18.3
WORKDIR /pokeguesser
COPY . /pokeguesser/

RUN apk update

RUN apk add python3 py3-pip
RUN pip install "fastapi[all]" fastapi_utils "uvicorn[standard]" pyserde

RUN apk add nodejs npm
RUN npm install -g npm

WORKDIR /pokeguesser/front
RUN npm install
RUN npm run build


WORKDIR /pokeguesser
CMD ["uvicorn", "Guesser:app", "--host", "0.0.0.0", "--port", "1234"]

EXPOSE 1234
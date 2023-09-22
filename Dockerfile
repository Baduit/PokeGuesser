FROM ubuntu:22.04 
WORKDIR /pokeguesser
COPY . /pokeguesser/

ENV DEBIAN_FRONTEND noninteractive
RUN apt update

RUN apt install python3 python3-pip -y
RUN pip install "fastapi[all]" fastapi_utils "uvicorn[standard]" pyserde


# All this shit because node and the whole js ecosystem is shit
ENV NVM_DIR /usr/local/nvm
ENV NODE_VERSION v16.17.0
RUN mkdir -p /usr/local/nvm && apt-get update && apt-get install curl -y
RUN curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash
RUN /bin/bash -c "source $NVM_DIR/nvm.sh && nvm install $NODE_VERSION && nvm use --delete-prefix $NODE_VERSION"
ENV NODE_PATH $NVM_DIR/versions/node/$NODE_VERSION/bin
ENV PATH $NODE_PATH:$PATH

RUN apt install npm -y

WORKDIR /pokeguesser/front
RUN npm install
RUN npm run build


WORKDIR /pokeguesser
CMD ["uvicorn", "Guesser:app", "--host", "0.0.0.0", "--port", "1234"]

EXPOSE 1234
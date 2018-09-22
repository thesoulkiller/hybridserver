# Hybrid Server
## Introduction

## Setup
### Dependencies
Coded with boost version 1.62 (From version 1.66 io_service class is changed to io_context)
`apt install make g++ libboost-all-dev sqlite3 libsqlite3-dev libjsoncpp-dev libcppnetlib-dev openssl`

### Compilation
make

### SSL Setup
1. DHPARAM File - for key exchange (Diffie-Hellman)
`openssl dhparam -out dhparam4096.pem 4096`
2. Generate a private key and csr for CA
`openssl req -new -newkey rsa:4096 -nodes -out ca.csr -keyout ca.key`
3. Generate a self-signed certificate for CA
`openssl x509 -signkey ca.key -in ca.csr -req -days 365 -out ca.crt`
4. Generate server private key and certificate request
```openssl genrsa -out server.key 1024
openssl req -new -key server.key -out server.csr```
5. Sign certificate with CA and generate public certificate for server
`openssl x509 -req -days 365 -in server.csr -out server.crt -CA ca.crt -CAkey ca.key -set_serial 01`
6. Make a chain file with CA and server public certificates to use it later in client
`cat server.crt ca.crt > ca-chain.cert.pem`

More to learn on:
**https://jamielinux.com/docs/openssl-certificate-authority/index.html**

### Database Setup
Enter postgreSQL shell with the postgres user
`$ sudo -u postgres psql`
Create new user with createdb permission
`postgres=# CREATE USER repomaster WITH password 'repomaster' CREATEDB;`

## What to learn ?
- Nonblocking operations to make the service available
- Encapsulate network read/write operations
- Basic PostgreSQL connector implementation
- Basic SSL implementation with trusted chain
- Power of Boost
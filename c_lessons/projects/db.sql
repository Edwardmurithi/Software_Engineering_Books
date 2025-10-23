CREATE DATABASE bank_system;

USE bank_system;

CREATE TABLE customers (
    accountNumber INT PRIMARY KEY,
    holderName VARCHAR(50),
    balance FLOAT,
    transactionHistory TEXT
);


gcc bank_mysql.c -o bank_mysql -lmysqlclient
./bank_mysql
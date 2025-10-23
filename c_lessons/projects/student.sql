CREATE DATABASE student_system;

USE student_system;

CREATE TABLE students (
    studentID INT PRIMARY KEY,
    fullName VARCHAR(100),
    course VARCHAR(50),
    yearOfStudy INT,
    gpa FLOAT
);

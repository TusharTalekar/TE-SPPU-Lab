DROP TABLE Student;
DROP TABLE PlacementDrive;
DROP TABLE Training;

CREATE TABLE Student(
    s_id INT PRIMARY KEY,
    Drive_id INT,
    T_id INT,
    s_name varchar(255),
    CGPA INT,
    s_branch varchar(255),
    s_dob DATE 
);
CREATE TABLE PlacementDrive(
    Drive_id INT PRIMARY KEY,
    Pcompany_name varchar(255),
    package DECIMAL(10,2),
    location varchar(255)
);
CREATE TABLE Training(
    T_id INT PRIMARY KEY,
    Tcompany_name varchar(255),
    T_fee DECIMAL(10,2),
    T_year YEAR
);



-- 1. Insert at least 10 records in the Student table and insert other tables accordingly.

INSERT INTO PlacementDrive (Drive_id, Pcompany_name, package, location) VALUES
(1, 'TCS', 85000.00, 'Pune'),
(2, 'Infosys', 50000.00, 'Mumbai'),
(3, 'Wipro', 65000.00, 'Pune'),
(4, 'TCS', 90000.00, 'Mumbai'),       
(5, 'Capgemini', 75000.00, 'Pune'),
(6, 'Infosys', 60000.00, 'Pune'),     
(7, 'Tech Mahindra', 55000.00, 'Mumbai'),
(8, 'Accenture', 80000.00, 'Mumbai'),
(9, 'IBM', 95000.00, 'Pune'),
(10, 'Cognizant', 40000.00, 'Mumbai');

INSERT INTO Training (T_id, Tcompany_name, T_fee, T_year) VALUES
(1, 'Udemy', 400.00, 2018),
(2, 'Coursera', 600.00, 2019),
(3, 'edX', 550.00, 2020),
(4, 'Pluralsight', 700.00, 2021),
(5, 'LinkedIn Learning', 650.00, 2017),
(6, 'Skillshare', 300.00, 2022),
(7, 'Codecademy', 500.00, 2023),
(8, 'Khan Academy', 0.00, 2018),
(9, 'DataCamp', 450.00, 2019),
(10, 'Udacity', 800.00, 2020);

INSERT INTO Student (s_id, Drive_id, T_id, s_name, CGPA, s_branch, s_dob) VALUES
(1, 1, 1, 'Amit Sharma', 9, 'AIDS', '2000-02-14'),
(2, 2, 2, 'Divya Patel', 8, 'IT', '2001-11-22'),
(3, 3, 3, 'Aditya Verma', 7, 'ECE', '1999-07-19'),
(4, 4, 4, 'Deepak Singh', 10, 'E&TC', '2002-01-30'),
(5, 5, 5, 'Anjali Kumar', 6, 'Computer', '2000-05-10'),
(6, 6, 6, 'Dinesh Reddy', 8, 'E&TC', '1998-09-25'),
(7, 7, 7, 'Akash Mehta', 9, 'Computer', '2001-08-14'),
(8, 8, 8, 'Darshan Joshi', 7, 'IT', '1999-12-03'),
(9, 9, 9, 'Ayesha Khan', 6, 'E&TC', '2000-06-07'),
(10, 10, 10, 'Dhruv Patel', 10, 'IT', '2002-03-18');




-- 2. Display all students details with branch ‘Computer ‘and ‘It’ and student name starting with ‘a' or 'd'.
SELECT *
FROM Student 
WHERE (s_branch = "Computer" OR s_branch ="IT") 
AND (s_name LIKE 'a%' OR s_name LIKE 'd%');


-- 3. list the number of different companies. (use of distinct)
SELECT DISTINCT Pcompany_name  
FROM PlacementDrive;
SELECT COUNT(DISTINCT Pcompany_name)  
FROM PlacementDrive;

SELECT Pcompany_name,COUNT(Pcompany_name) 
FROM PlacementDrive 
GROUP BY Pcompany_name;


-- 4. Give 15% increase in fee of the Training whose joining year is 2019.
UPDATE Training 
SET T_fee = T_fee*  1.15
WHERE T_year = 2019;


-- 5. Delete Student details having CGPA score less than 7.
DELETE FROM Student
WHERE CGPA < 7;


-- 6. Find the names of companies belonging to Pune or Mumbai
SELECT Pcompany_name,location
FROM PlacementDrive
WHERE location IN ('Pune', 'Mumbai');


-- 7. Find the student’s name who joined training in 1-1-2019 as well as in 1-1-2021
SELECT s_name
FROM Student
WHERE T_id IN (
    SELECT T_id FROM Training WHERE T_year IN (2019, 2021)
);


-- 8. Find the student’s name having maximum CGPA score and names of students having CGPA score between 7 to 9.
SELECT s_name
FROM Student
WHERE CGPA = (SELECT MAX(CGPA) FROM Student);

SELECT s_name, CGPA
FROM Student
WHERE CGPA >= 7 AND CGPA <= 9;

SELECT s_name, CGPA
FROM Student 
WHERE CGPA BETWEEN 7 AND 9;


-- 9. Display all Student name with T_id with decreasing order of Fees
SELECT s.s_name, s.T_id
FROM Student s
ORDER BY (
    SELECT T_fee FROM Training t WHERE t.T_id = s.T_id
) DESC;


-- 10. Display Pcompany_name, S_name, location and Package with Package 30K, 40K and 50k
SELECT p.Pcompany_name, s.s_name, p.location, p.package
FROM Student s, PlacementDrive p
WHERE s.Drive_id = p.Drive_id
AND p.package IN (30000, 40000, 50000);



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


-- Insert into PlacementDrive
INSERT INTO PlacementDrive (Drive_id, Pcompany_name, package, location) VALUES
(101, 'Microsoft', 500000.00, 'Thane'),
(102, 'Google', 800000.00, 'Mumbai'),
(103, 'TCS', 400000.00, 'Pune'),
(104, 'Infosys', 10000.00, 'Delhi'),
(105, 'Amazon', 500000.00, 'Hyderabad');

-- Insert into Training
INSERT INTO Training (T_id, Tcompany_name, T_fee, T_year) VALUES
(201, 'Microsoft', 25000.00, 2015),
(202, 'Google', 15000.00, 2013),
(203, 'Infosys', 30000.00, 2011),
(204, 'TCS', 10000.00, 2014),
(205, 'Amazon', 22000.00, 2015);

-- Insert into Student
INSERT INTO Student (s_id, Drive_id, T_id, s_name, CGPA, s_branch, s_dob) VALUES
(1, 101, 203, 'Shantanu', 8, 'CSE', '2000-03-01'),
(2, 101, 203, 'Ravi', 7, 'IT', '1999-05-11'),
(3, 102, 202, 'Priya', 9, 'ECE', '2001-06-21'),
(4, 103, 204, 'Anjali', 7, 'CSE', '1998-10-03'),
(5, 104, 205, 'Amit', 6, 'MECH', '2000-12-15'),
(6, 105, 201, 'Neha', 8, 'CSE', '2001-07-23'),
(7, 105, 205, 'Kiran', 9, 'EEE', '1999-08-10'),
(8, 102, 201, 'Rahul', 6, 'IT', '2002-09-09'),
(9, 101, 201, 'Megha', 7, 'CSE', '1998-01-01'),
(10, 104, 203, 'Sneha', 8, 'ECE', '1999-11-05');
    



CREATE VIEW Student1 AS
SELECT *
FROM Student
WHERE T_id IN(202,203,204);

CREATE VIEW Student2 AS
SELECT *
FROM Student
WHERE Drive_id IN(101,103,104);

CREATE VIEW Training1 AS
SELECT * 
FROM Training 
WHERE T_id IN(202,203); 

CREATE VIEW Placement1 AS
SELECT * 
FROM PlacementDrive
WHERE Drive_id IN(101,102,105);


select Student1.s_id, Student1.s_name, Student1.CGPA, Student1.s_branch, Student1.s_dob,
       Training1.T_id, Training1.Tcompany_name, Training1.T_fee, Training1.T_year
from Student1
left join Training1
ON Student1.T_id = Training1.T_id;

select Student1.s_id, Student1.s_name, Student1.CGPA, Student1.s_branch, Student1.s_dob,
       PlacementDrive.Drive_id, PlacementDrive.Pcompany_name , PlacementDrive.package , PlacementDrive.location
from Student1
right join PlacementDrive
ON Student1.Drive_id = PlacementDrive.Drive_id;

select Student2.s_id, Student2.s_name, Student2.CGPA, Student2.s_branch, Student2.s_dob,
       Placement1.Drive_id, Placement1.Pcompany_name , Placement1.package , Placement1.location
from Placement1
left JOIN Student2 
ON Student2.Drive_id = Placement1.Drive_id
UNION
select Student2.s_id, Student2.s_name, Student2.CGPA, Student2.s_branch, Student2.s_dob,
       Placement1.Drive_id, Placement1.Pcompany_name , Placement1.package , Placement1.location
from Placement1
right JOIN Student2 
ON Student2.Drive_id = Placement1.Drive_id;

select Student2.s_id, Student2.s_name, Student2.CGPA, Student2.s_branch, Student2.s_dob,
       Placement1.Drive_id, Placement1.Pcompany_name , Placement1.package , Placement1.location
from Placement1
cross join Student2 ;




-- 1. Find the Student details and Placement details using NATURAL JOIN.
SELECT * 
from Student
NATURAL JOIN PlacementDrive; 


-- 2. Find all the student details with company_name who have conducted in same drive
SELECT PlacementDrive.Drive_id,PlacementDrive.Pcompany_name,Student.*
from PlacementDrive
RIGHT join Student
on Student.Drive_id=PlacementDrive.Drive_id
order by PlacementDrive.Drive_id;


-- 3. List all the Student name and Student branch of Student having package 5 LPA
select Student.s_name,Student.s_branch
from Student
inner join PlacementDrive 
ON Student.Drive_id=PlacementDrive.Drive_id  and  PlacementDrive.package=500000;


-- 4. List all the student names ,company_name having T_fee more than 20000
select Student.s_name,Training.Tcompany_name
from Student
inner join Training
ON Student.T_id = Training.T_id  and  Training.T_fee > 20000;


-- 5. Display all training details attended by “shantanu” in year 2011
select Student.s_name,Training.*
from Student
inner join Training
ON Student.s_name="shantanu" and Student.T_id = Training.T_id  and Training.t_year=2011;


-- 6. list the total number of companies who conduct training before 2015
SELECT COUNT(DISTINCT Tcompany_name) 
AS total_companies
FROM Training
WHERE T_year<2015;

select Distinct Tcompany_name 
from Training
WHERE T_year<2015;



-- 7. List the students name with company ‘Microsoft’ and location ’Thane’
SELECT Student.s_name,PlacementDrive.Pcompany_name,PlacementDrive.location
from Student
right join PlacementDrive
on Student.Drive_id=PlacementDrive.Drive_id 
where PlacementDrive.Pcompany_name="Microsoft" and PlacementDrive.location="Thane";


CREATE VIEW StudentTrainingView AS
SELECT Student.s_id, Student.s_name, Student.CGPA, Student.s_branch, Student.s_dob,
       Training.T_id, Training.Tcompany_name, Training.T_fee, Training.T_year
FROM Student
INNER JOIN Training ON Student.T_id = Training.T_id;

-- 8. Find the names of all Students who have joined ‘Microsoft ‘ training in 2015 .
select s_name 
from StudentTrainingView 
where Tcompany_name="Microsoft" 
and T_year=2015;

-- 9. Create a view showing the Student and Training details.
select * 
from StudentTrainingView; 

-- 10. Perform Manipulation on simple view-Insert, update, delete, drop view.
drop view if exists training202;
drop view if exists training203;

CREATE VIEW training203 as
select *
from Student 
where T_id=203; 
CREATE VIEW training202 as
select *
from Student 
where T_id=202; 


UPDATE training203
set T_id=202;



delete from training202
where s_branch="IT";

Select *from Student; 

select * from training203;

select * from training202;

drop view if exists training203;
drop view if exists training202;

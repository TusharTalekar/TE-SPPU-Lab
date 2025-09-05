CREATE TABLE Employee(
    Emp_id  INTEGER PRIMARY KEY,
    Dept_id  INTEGER,
    Emp_fname  TEXT,
    Emp_lname   TEXT,
    Emp_Position  TEXT,
    Emp_salary   INTEGER,
    Emp_JoinDate    TEXT
);
CREATE TABLE Dept(
    Dept_id   INTEGER  PRIMARY KEY,
    Dept_name  TEXT,
    Dept_location TEXT
);
CREATE TABLE Project(
    Proj_id  INTEGER  PRIMARY KEY,
    Dept_id  INTEGER,
    Proj_Name  TEXT,
    Proj_Location  TEXT,
    Proj_cost  INTEGER,
    Proj_year  INTEGER
);


INSERT INTO Dept VALUES
    (1, 'Human Resources', 'Pune'),
    (2, 'Engineering', 'Kolkata'),
    (3, 'Marketing', 'Mumbai'),
    (4, 'Finance', 'Bangalore'),
    (5, 'Legal', 'Chennai'),
    (6, 'IT', 'Hyderabad'),
    (7, 'Sales', 'Delhi'),
    (8, 'R&D', 'Ahmedabad'),
    (9, 'Computer', 'Nagpur'),
    (10, 'Customer Service', 'Jaipur');
    
INSERT INTO Employee VALUES
    (101, 1, 'Ram', 'Pavar', 'HR Manager', 75000, '2020-03-15'),
    (102, 2, 'Shyam', 'Kapoor', 'Software Engineer', 95000, '2019-07-01'),
    (103, 2, 'Tom', 'Joshi', 'DevOps Engineer', 90000, '2021-01-12'),
    (104, 3, 'Jerry', 'Desai', 'Marketing Lead', 85000, '2018-10-05'),
    (105, 4, 'Aditya', 'Pillai', 'Financial Analyst', 80000, '2022-05-20'),
    (106, 6, 'Hema', 'Sharma', 'HR Executive', 72000, '2023-06-01'),
    (107, 2, 'Rahul', 'Verma', 'Backend Developer', 91000, '2021-09-15'),
    (108, 6, 'Sneha', 'Rao', 'Data Scientist', 97000, '2022-11-10'),
    (109, 3, 'Arjun', 'Mehta', 'Brand Manager', 84000, '2020-12-20'),
    (110, 6, 'Kavya', 'Pillai', 'Financial Consultant', 83000, '2023-02-01'),
    (111, 9, 'Vivek', 'Desai', 'HR Manager', 68000, '2024-01-10'),
    (112, 3, 'Meera', 'Nair', 'Marketing Lead', 75000, '2022-04-05'),
    (113, 2, 'Rohan', 'Joshi', 'Full Stack Developer', 94000, '2021-07-18'),
    (115, 9, 'Priyanka', 'Kapoor', 'Marketing Analyst', 79000, '2023-05-25');
    
INSERT INTO Project VALUES
    (201, 2, 'AI Development', 'Kolkata', 200000, 2023),
    (202, 3, 'Product Launch', 'Mumbai', 150000, 2007),
    (203, 4, 'Budget Planning', 'Banglore', 100000, 2023),
    (204, 1, 'Employee Wellness', 'Pune', 50000, 2021),
    (205, 2, 'Cloud Migration', 'Kolkata', 180000, 2004),
    (206, 3, 'Ad Campaign', 'Mumbai', 120000, 2023),
    (207, 4, 'Investment Strategy', 'Bangalore', 110000, 2005),
    (208, 5, 'Compliance Overhaul', 'Pune', 95000, 2023),
    (209, 6, 'IT Infrastructure Upgrade', 'Hyderabad', 130000, 2007),
    (210, 7, 'Sales Expansion', 'Delhi', 160000, 2023),
    (211, 8, 'Product R&D Phase 2', 'Ahmedabad', 190000, 2025),
    (212, 9, 'Warehouse Automation', 'Nagpur', 140000, 2022),
    (213, 10, 'Customer Feedback System', 'Jaipur', 85000, 2023),
    (214, 2, 'AI Chatbot Development', 'Kolkata', 210000, 2025);



-- Display all Employee details with Department ‘Computer’ and ‘IT’ and Employee first name starting with 'p' or 'h'.
SELECT *
    FROM Employee 
WHERE(Emp_fname LIKE 'p%' OR Emp_fname LIKE 'h%')
AND Dept_id IN (
SELECT Dept_id FROM Dept
WHERE(Dept_name = 'Computer' OR Dept_name = 'IT')
);


-- lists the number of different Employee Positions.
SELECT COUNT(DISTINCT Emp_Position) AS NumberOfPositions
FROM Employee;
SELECT DISTINCT Emp_Position
FROM Employee;


-- Give 10% increase in Salary of the Employee whose joining year is before 1985.
UPDATE Employee
SET Emp_salary = ROUND(Emp_salary * 1.10)
WHERE YEAR(STR_TO_DATE(Emp_JoinDate, '%Y-%m-%d')) < 1985;


-- Delete Department details which location is ‘Mumbai’.
DELETE FROM Dept WHERE Dept_location = 'Mumbai';
SELECT * FROM dept;


-- Find the names of Projects with location ‘pune’ .
SELECT Proj_id, Proj_Name, Proj_Location FROM Project WHERE Proj_Location = 'Pune'; 


-- Find the project having cost in between 100000 to 500000.
SELECT Proj_id, Proj_Name, Proj_cost 
FROM Project 
WHERE Proj_cost >= 100000 
AND Proj_cost <= 500000;


-- Find the project having maximum price and find average of Project cost
SELECT *
    FROM Project
WHERE Proj_cost = (SELECT MAX(Proj_cost) FROM Project);
SELECT AVG(Proj_cost) AS Average_Project_Cost
FROM Project;


-- Display all employees with Emp _id and Emp name in decreasing order of Emp_lname
SELECT Emp_id, Emp_fname, Emp_lname
FROM Employee
ORDER BY Emp_lname DESC;


-- Display Proj_name,Proj_location ,Proj_cost of all project started in 2004,2005,2007
SELECT Proj_Name, Proj_Location, Proj_cost, Proj_year
FROM Project
WHERE Proj_year IN(2004, 2005, 2007);



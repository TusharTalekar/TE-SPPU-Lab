-- Named PL/SQL Block: PL/SQL Stored Procedure and Stored Function.
-- Write a Stored Procedure namely proc_Grade for the categorization of student. If marks scored by students in examination is <=1500 and marks>=990 then student will be placed in distinction category if marks scored are between 989 and 900 category is first class, if marks 899 and 825 category is Higher Second Class
-- Write a PL/SQL block to use procedure created with above requirement.
-- Stud_Marks(name, total_marks)
-- Result(Roll, Name, Class)
-- Note: Instructor will frame the problem statement for writing stored procedure and Function in line with above statement.


-- In mysql

DROP TABLE IF EXISTS Stud_Marks;
DROP TABLE IF EXISTS Result;

CREATE TABLE Stud_Marks(
    Roll INT PRIMARY KEY,
    Name varchar(255),
    total_marks INT
);

CREATE TABLE Result(
    Roll INT PRIMARY KEY,
    Name varchar(255),
    Class varchar(255)
);


DROP PROCEDURE IF EXISTS proc_Grade;

DELIMITER $$

CREATE PROCEDURE proc_Grade(
    IN stud_roll INT
)
BEGIN
    DECLARE stud_class varchar(255);
    DECLARE stud_name varchar(255);
    DECLARE sMarks INT;

    DECLARE EXIT HANDLER FOR NOT FOUND
    BEGIN
        SELECT 'Error occurred: Invalid Roll_no not found.' AS ErrorMessage;
    END;

    IF stud_roll <= 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid Roll number: roll number should be positive.';
    END IF;


    SELECT Name, total_marks INTO stud_name,sMarks
    FROM Stud_Marks
    WHERE Roll=stud_roll;


    IF sMarks <= 0 OR sMarks > 1500 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid marks : only between 0 to 1500 allowed.';
    END IF;
    
    IF stud_name REGEXP '[^a-zA-Z ]' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid Name : only alphabets and spaces allowed.';
    END IF;
    
    


    IF sMarks<=1500 AND sMarks>=990 THEN
        SET stud_class = "Distinction";
    ELSEIF sMarks<=989 AND sMarks>=900 THEN 
        SET stud_class = "First Class";
    ELSEIF sMarks<=899 AND sMarks>=825 THEN 
        SET stud_class = "Higher Second Class";
    ELSE
        SET stud_class = "Second Class";
    END IF;

    INSERT INTO Result VALUES(stud_roll,stud_name,stud_class);

    SELECT CONCAT(stud_roll," ",stud_name,' in  ',stud_class,'  with  ',sMarks,"  marks.") AS Message;
END $$

DELIMITER ;


INSERT INTO Stud_Marks VALUES (101,"Shradhha",1200 );
INSERT INTO Stud_Marks VALUES (102,"Prem", 950);
INSERT INTO Stud_Marks VALUES (103,"Naman", 850);
INSERT INTO Stud_Marks VALUES (104,"Viraj", 400);
INSERT INTO Stud_Marks VALUES (105,"Manoj",1600 );
INSERT INTO Stud_Marks VALUES (106,"Ma$noj", 300);
INSERT INTO Stud_Marks VALUES ("ABC","Ma$noj", 300);
INSERT INTO Stud_Marks VALUES (107,"Manoj",-1200 );
INSERT INTO Stud_Marks VALUES (108,"Manoj",500);
INSERT INTO Stud_Marks VALUES (-109,"Manoj",500);

SELECT * FROM Stud_Marks;

CALL proc_Grade(101);
CALL proc_Grade(102);
CALL proc_Grade(103);
CALL proc_Grade(104);
CALL proc_Grade(105);
CALL proc_Grade(106);
CALL proc_Grade(107);
CALL proc_Grade(108);
CALL proc_Grade(-109);
CALL proc_Grade(-110);



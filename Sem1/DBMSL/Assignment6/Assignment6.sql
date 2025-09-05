-- Cursors(Implicit,Explicit, Cursor for loop, parameterized cursor )
-- Write a PL/SQL block of code using parameterized cursor that will merge the data available in newly created table 
-- N_EmpId with the data available in table O_EmpId
-- if data in first table already exist in second table then that data should be skipped

DROP TABLE IF EXISTS N_EmpId;
DROP TABLE IF EXISTS O_EmpId; 

CREATE TABLE N_EmpId(
    empid INT PRIMARY KEY,
    empname varchar(255)
);

CREATE TABLE O_EmpId(
    empid INT PRIMARY KEY,
    empname varchar(255)
);

INSERT INTO N_EmpId VALUES
(1,"Hema"),
(2,"Kumar"),
(3,"Jay");

INSERT INTO O_EmpId VALUES
(3,"Vaibhav"),
(5,"Ramesh"),
(6,"Vinesh");



DELIMITER //

CREATE PROCEDURE merge_emp()
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE v_empid INT;
    DECLARE v_empname varchar(255);

    DECLARE curr CURSOR FOR SELECT empid, empname FROM N_EmpId;
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done=TRUE;

    open curr;

    read_loop:LOOP 
        FETCH curr INTO v_empid, v_empname;
        IF done THEN
            LEAVE read_loop;
        END IF;

        IF NOT EXISTS (SELECT 1 from O_EmpId WHERE empid=v_empid) THEN
            INSERT into O_EmpId (empid,empname) VALUES (v_empid,v_empname);
        END IF;

    END LOOP;

    CLOSE curr;

END //

DELIMITER ;

SELECT * FROM N_EmpId;
SELECT * FROM O_EmpId;
CALL merge_emp();
SELECT * FROM O_EmpId;




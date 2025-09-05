-- Borrower(Roll_no,Name,DateofIssue,NameofBook ,Status)
-- Fine(Roll_no,Date,Amt)
-- Accept roll no and NameofBook
-- Check no of days (from DateofIssue)
-- if days are between 15 to 30 fine amount will be 5rs per day
-- if days >30 fine will be 50 rs per day
-- after submitting book status will change to I to R .If condition of fine ,details will be astoresd into  fine table
-- also handles exception by named exception handler or user define exception handler

DROP TABLE IF EXISTS Borrower;
DROP TABLE IF EXISTS Fine;

CREATE TABLE Borrower(
    Roll_no INT PRIMARY KEY,
    Name varchar(255),
    DateofIssue DATE,
    NameofBook varchar(255),
    Status varchar(255)
);

CREATE TABLE Fine(
    Roll_no INT ,
    Date DATE,
    Amt INT
);




INSERT INTO Borrower (Roll_no,Name, DateofIssue, NameofBook, Status) VALUES (101,"Shradhha", '2025-07-10', 'Data Structures', 'I');
INSERT INTO Borrower (Roll_no,Name, DateofIssue, NameofBook, Status) VALUES (102,"Prem", '2025-06-15', 'Algorithms', 'I');
INSERT INTO Borrower (Roll_no,Name, DateofIssue, NameofBook, Status) VALUES (103,"Naman", '2025-07-20', 'Operating Systems', 'I');
INSERT INTO Borrower (Roll_no,Name, DateofIssue, NameofBook, Status) VALUES (-104,"Viraj", '2025-06-25', 'Databases', 'I');
INSERT INTO Borrower (Roll_no,Name, DateofIssue, NameofBook, Status) VALUES (105,"Ma$noj", '2025-07-01', 'Networking', 'I');


DROP PROCEDURE IF EXISTS ReturnBook;


DELIMITER //

CREATE PROCEDURE ReturnBook(
    IN roll INT,
    IN bookName VARCHAR(255)
)
BEGIN
    DECLARE v_DateofIssue DATE;
    DECLARE v_Status CHAR(1);
    DECLARE studName CHAR(255);
    DECLARE v_Days INT; 
    DECLARE v_FineAmt INT DEFAULT 0;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        SELECT 'Error occurred: Invalid Roll_no or Book Name or other error.' AS ErrorMessage;
    END;
    DECLARE EXIT HANDLER FOR NOT FOUND
    BEGIN
        SELECT 'No record found for given Roll_no and Book Name.' AS ErrorMessage;
    END;

    IF bookName REGEXP '[0-9]' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid book name. It should not contain numbers.';
    END IF;
    IF roll <= 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid Roll number: cannot negative.';
    END IF;
    

    SELECT DateofIssue, Status,Name INTO v_DateofIssue, v_Status, studName
    FROM Borrower
    WHERE Roll_no = roll AND NameofBook = bookName;


    IF studName REGEXP '[^a-zA-Z ]' THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid Name: only alphabets and spaces allowed.';
    END IF;
    IF v_DateofIssue > CURDATE() THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid DateofIssue: cannot be a future date.';
    END IF;




    IF v_Status = 'I' THEN
        SET v_Days = DATEDIFF(CURDATE(), v_DateofIssue);


        IF v_Days BETWEEN 15 AND 30 THEN
            SET v_FineAmt = (v_Days - 15) * 5;
        ELSEIF v_Days > 30 THEN
            SET v_FineAmt = (v_Days - 30) * 50+ (15 * 5);
        ELSE
            SET v_FineAmt = 0;
        END IF;


        IF v_FineAmt > 0 THEN
            INSERT INTO Fine (Roll_no, Date, Amt) VALUES (roll, CURDATE(), v_FineAmt);
        END IF;


        UPDATE Borrower
        SET Status = 'R'
        WHERE Roll_no = roll AND NameofBook = bookName;


        SELECT CONCAT('Book returned successfully. Fine amount: Rs. ', v_FineAmt) AS Message;
    END IF;

END //


DELIMITER ;






CALL ReturnBook(101,"Data Structures");
CALL ReturnBook(102,"Algorithms");
CALL ReturnBook(103,"Operating Systems");
CALL ReturnBook(107,"Operating Systems");



-- Database Trigger (All Types: Row level and Statement level triggers, Before and After Triggers).
-- Write a database trigger on Library table. The System should keep track of the records that are being updated or deleted. The old value of updated or deleted records should be added in Library Audit table.
-- Note: Instructor will Frame the problem statement for writing PL/SQL block for all types of Triggers in line with above statement.




DROP TABLE IF EXISTS Library;
DROP TABLE IF EXISTS Library_Audit;

CREATE TABLE Library(
    book_id INT PRIMARY KEY,
    title varchar(255),
    author varchar(255),
    published_year INT
);


CREATE TABLE Library_Audit (
    audit_id INT AUTO_INCREMENT PRIMARY KEY,
    book_id INT,
    title VARCHAR(255),
    author VARCHAR(255),
    published_year INT,
    operation VARCHAR(10), 
    operation_date DATETIME DEFAULT CURRENT_TIMESTAMP
);


DELIMITER //

CREATE TRIGGER before_library_update
BEFORE UPDATE ON Library
FOR EACH ROW 
BEGIN
    INSERT INTO Library_Audit (book_id, title, author, published_year, operation)
    VALUES (OLD.book_id,OLD.title,OLD.author,OLD.published_year,'update');
END //


CREATE TRIGGER before_library_delete
BEFORE DELETE ON Library 
FOR EACH ROW 
BEGIN
    INSERT INTO Library_Audit (book_id, title, author, published_year, operation)
    VALUES (OLD.book_id,OLD.title,OLD.author,OLD.published_year,'delete');
END //


DELIMITER ;



INSERT INTO Library
(book_id,title,author,published_year) VALUES 
(201, 'Kosala', 'Bhalchandra Nemade', 1963),
(202, 'Yayati', 'Vishnu Sakharam Khandekar', 1959),
(203, 'Batatyachi Chaal', 'P. L. Deshpande', 1967),
(204, 'Mrityunjay', 'Shivaji Sawant', 1967),
(205, 'Gulam Ani Itar Katha', 'Nirmala Deshpande', 1985);



UPDATE Library
SET published_year = 1964
WHERE title='Kosala';

DELETE FROM Library
WHERE title='Yayati';


SELECT * FROM Library;
SELECT * FROM Library_Audit;
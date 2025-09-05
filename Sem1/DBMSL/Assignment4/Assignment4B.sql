-- Write a PL/SQL code block to calculate the area of a circle for a value of radius varying from 5 to 9. Store the radius and the corresponding values of calculated area in an empty table named areas, consisting of two columns, radius and area.
-- Note: Instructor will frame the problem statement for writing PL/SQL block in line with above statement.


DROP TABLE IF EXISTS Areas;


CREATE TABLE Areas(
    Radius INT PRIMARY KEY,
    Area FLOAT(12,2)
);


DROP PROCEDURE IF EXISTS calculate_areas;


DELIMITER //

CREATE PROCEDURE calculate_areas()
BEGIN
    DECLARE r INT DEFAULT 5;
    DECLARE a DOUBLE;

    WHILE r <= 9 DO
        SET a = PI() * POW(r, 2);
        INSERT INTO Areas(Radius, Area) VALUES (r, a);
        SET r = r + 1;
    END WHILE;
END //

DELIMITER ;

    
CALL calculate_areas();

SELECT * FROM Areas;
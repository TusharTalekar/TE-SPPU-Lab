

import java.sql.*;
import java.util.Scanner;

public class StudentDB {

    private Connection conn;
    private Scanner sc;

    public StudentDB() {
        sc = new Scanner(System.in);

        // DB Connection
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            conn = DriverManager.getConnection(
                    "jdbc:mysql://10.10.14.233:3306/te31474_db", "te31474", "te31474"
                    // "jdbc:mysql://localhost:3306/StudentDB", "root", "tushar@12"
            );
            System.out.println("Connected to DB!");
        } catch (Exception e) {
            System.out.println("DB Error: " + e.getMessage());
            System.exit(0);
        }
    }

    private void addStudent() {
        try {
            System.out.print("Enter Name: ");
            String name = sc.nextLine();
            System.out.print("Enter Age: ");
            int age = Integer.parseInt(sc.nextLine());

            String sql = "INSERT INTO students(name, age) VALUES(?, ?)";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setString(1, name);
            ps.setInt(2, age);
            ps.executeUpdate();
            System.out.println("Added student: " + name);
        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
        }
    }

    private void updateStudent() {
        try {
            System.out.print("Enter Student ID to update: ");
            int id = Integer.parseInt(sc.nextLine());
            System.out.print("Enter New Name: ");
            String name = sc.nextLine();
            System.out.print("Enter New Age: ");
            int age = Integer.parseInt(sc.nextLine());

            String sql = "UPDATE students SET name=?, age=? WHERE id=?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setString(1, name);
            ps.setInt(2, age);
            ps.setInt(3, id);
            int rows = ps.executeUpdate();
            if (rows > 0) {
                System.out.println("Updated student ID: " + id);
            } else {
                System.out.println("Student ID not found.");
            }
        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
        }
    }

    private void deleteStudent() {
        try {
            System.out.print("Enter Student ID to delete: ");
            int id = Integer.parseInt(sc.nextLine());

            String sql = "DELETE FROM students WHERE id=?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setInt(1, id);
            int rows = ps.executeUpdate();
            if (rows > 0) {
                System.out.println("Deleted student ID: " + id);
            } else {
                System.out.println("Student ID not found.");
            }
        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
        }
    }

    private void viewStudents() {
        try {
            String sql = "SELECT * FROM students";
            Statement st = conn.createStatement();
            ResultSet rs = st.executeQuery(sql);
            System.out.println("ID | Name | Age");
            while (rs.next()) {
                System.out.println(rs.getInt("id") + " | " +
                        rs.getString("name") + " | " +
                        rs.getInt("age"));
            }
        } catch (Exception ex) {
            System.out.println("Error: " + ex.getMessage());
        }
    }

    private void menu() {
        while (true) {
            System.out.println("\n==== Student Database Menu ====");
            System.out.println("1. Add Student");
            System.out.println("2. Update Student");
            System.out.println("3. Delete Student");
            System.out.println("4. View All Students");
            System.out.println("5. Exit");
            System.out.print("Enter your choice: ");
            String choice = sc.nextLine();

            switch (choice) {
                case "1":
                    addStudent();
                    break;
                case "2":
                    updateStudent();
                    break;
                case "3":
                    deleteStudent();
                    break;
                case "4":
                    viewStudents();
                    break;
                case "5":
                    System.out.println("Exiting...");
                    try { conn.close(); } catch(Exception e) {}
                    System.exit(0);
                default:
                    System.out.println("Invalid choice. Try again.");
            }
        }
    }

    public static void main(String[] args) {
        StudentDB app = new StudentDB();
        app.menu();
    }
}

// package Assignment12;
import java.util.Scanner;
import com.mongodb.client.*;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Updates;
import org.bson.Document;
import org.bson.conversions.Bson;

public class App {

    public static MongoClient mongoClient;
    public static MongoDatabase db;

    public static void print(String s) {
        System.out.println(s);
    }

    public static boolean checker(int choice, int given) {
        return choice == given;
    }

public static void createConnection() throws Exception {
    // connect to local MongoDB server
    mongoClient = MongoClients.create("mongodb://localhost:27017");
    db = mongoClient.getDatabase("WaterDB"); // you can choose any db name
    System.out.println("✅ Connected to local database: " + db.getName());

    // Create collection if not exists
    boolean exists = false;
    for (String name : db.listCollectionNames()) {
        if (name.equals("WaterData")) {
            exists = true;
            break;
        }
    }
    if (!exists) {
        db.createCollection("WaterData");
        System.out.println("🆕 Created collection: WaterData");
    } else {
        System.out.println("📘 Using existing collection: WaterData");
    }
}

    public static void insertData(Scanner sc) {
        MongoCollection<Document> collection = db.getCollection("WaterData");

        print("Enter City Name: ");
        sc.nextLine(); // clear buffer
        String city = sc.nextLine();

        print("Enter Population: ");
        int population = sc.nextInt();

        print("Enter Water Usage (MLD): ");
        double waterUsage = sc.nextDouble();

        sc.nextLine(); // clear buffer
        print("Enter Water Source: ");
        String waterSource = sc.nextLine();

        print("Enter Status (Sufficient / Scarce / Critical): ");
        String status = sc.nextLine();

        Document doc = new Document("city", city)
                .append("population", population)
                .append("waterUsage", waterUsage)
                .append("waterSource", waterSource)
                .append("status", status);

        collection.insertOne(doc);
        print("✅ Record inserted successfully!");
    }

    public static void getData(Scanner sc) {
        MongoCollection<Document> collection = db.getCollection("WaterData");
        print("\n📊 All Water Data Records:");
        for (Document doc : collection.find()) {
            System.out.println(doc.toJson());
        }
    }

    public static void updateData(Scanner sc) {
        MongoCollection<Document> collection = db.getCollection("WaterData");
        sc.nextLine();
        print("Enter City Name to Update: ");
        String city = sc.nextLine();

        print("Enter New Water Usage (MLD): ");
        double newUsage = sc.nextDouble();

        sc.nextLine();
        print("Enter New Status: ");
        String newStatus = sc.nextLine();

        Bson filter = Filters.eq("city", city);
        Bson updates = Updates.combine(
                Updates.set("waterUsage", newUsage),
                Updates.set("status", newStatus)
        );

        var result = collection.updateOne(filter, updates);
        if (result.getMatchedCount() > 0)
            print("✅ Record updated successfully!");
        else
            print("⚠️ City not found!");
    }

    public static void deleteData(Scanner sc) {
        MongoCollection<Document> collection = db.getCollection("WaterData");
        sc.nextLine();
        print("Enter City Name to Delete: ");
        String city = sc.nextLine();

        var result = collection.deleteOne(Filters.eq("city", city));
        if (result.getDeletedCount() > 0)
            print("🗑️ Record deleted successfully!");
        else
            print("⚠️ City not found!");
    }

    public static void main(String[] args) {
        try {
            createConnection();
        } catch (Exception e) {
            System.out.println("❌ Not able to Connect to Database");
            e.printStackTrace();
            return;
        }

        try (Scanner sc = new Scanner(System.in)) {
            while (true) {
                print("\n---------- Water Data Menu -----------");
                print("1. Get All Water Data");
                print("2. Insert New Water Data");
                print("3. Update Water Data");
                print("4. Delete Water Data");
                print("5. Exit");
                print("--------------------------------------");
                print("Enter choice: ");
                int choice = sc.nextInt();

                if (checker(choice, 1)) getData(sc);
                else if (checker(choice, 2)) insertData(sc);
                else if (checker(choice, 3)) updateData(sc);
                else if (checker(choice, 4)) deleteData(sc);
                else if (checker(choice, 5)) {
                    mongoClient.close();
                    print("🌿 Thank You !");
                    break;
                } else {
                    print("❌ Invalid Option!");
                }
            }
        }
    }
}

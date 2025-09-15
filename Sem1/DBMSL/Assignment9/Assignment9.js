// {
//  "_id": ObjectId("..."),
//  name: "Mobile",
//  category: "Electronics",
//  price: 20000,
//  stock: 50,
//  available: true
// }

// show dbs
// use te4

db.products.insertOne({
  name: "Laptop",
  category: "Electronics",
  price: 55000,
  stock: 25,
  available: true
});

db.products.insertMany([
  { name: "Mobile", category: "Electronics", price: 20000, stock: 50, available: true },
  { name: "Chair", category: "Furniture", price: 3000, stock: 100, available: true },
  { name: "Shoes", category: "Fashion", price: 1500, stock: 0, available: false }
]);


db.products.find();
db.products.find({ category: "Electronics" });
db.products.find({}, { name: 1, price: 1, _id: 0 });

db.products.find({ price: { $gt: 10000 } });
db.products.find({ price: { $lt: 5000 } });
db.products.find({ price: { $gte: 55000 } });

db.products.find({ $and: [{ category: "Electronics" }, { price: { $gt: 25000 } }] });
db.products.find({ $or: [{ name: "Chair" }, { category: "Electronics" }] });
db.products.find({ $nor: [{ price: { $gt: 3000 } }] });

db.products.updateMany(
  { category: "Fashion" },
  { $inc: { price: 1000 } }
);

db.products.deleteOne({ name: "Laptop" });

db.products.deleteMany({ available: false });

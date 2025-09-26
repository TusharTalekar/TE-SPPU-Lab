// MongoDB Aggregation and Indexing:
// Design and Develop MongoDB Queries using aggregation and indexing with suitable example using MongoDB


// $match → filters documents (like WHERE).
// $group → groups documents by a field.
// $sort → sorts documents.
// $project → selects specific fields or creates computed fields.
// $limit → limits number of documents.
// $count → counts documents.



db.products.aggregate([
    { $match: { category: "Electronics" } },
    {
        $group: {
            _id: "$category",
            avgPrice: { $avg: "$price" },
            totalStock: { $sum: "$stock" }
        }
    },
    { $sort: { avgPrice: -1 } },
    {
        $project: {
            _id: 0,
            Category: "$_id",
            "Average Price": "$avgPrice",
            "Total Stock": "$totalStock"
        }
    },
    { $limit: 1 }
]);



// Find only Electronics products
db.products.aggregate([
    { $match: { category: "Electronics" } }
]);

// Show only name and price, and add a GST field
db.products.aggregate([
    { $project: { _id: 0, name: 1, price: 1, gstPrice: { $multiply: ["$price", 1.18] } } }
]);
// Group by category and count total products
db.products.aggregate([
    { $group: { _id: "$category", totalProducts: { $sum: 1 } } }
]);

// Find average price of products per category
db.products.aggregate([
    { $group: { _id: "$category", avgPrice: { $avg: "$price" } } }
]);
// Sort products by price (highest first)
db.products.aggregate([
    { $sort: { price: -1 } }
]);
// Show top 5 expensive products
db.products.aggregate([
    { $sort: { price: -1 } },
    { $limit: 5 }
]);
// Skip first 5 and show next 5 products
db.products.aggregate([
    { $skip: 5 },
    { $limit: 5 }
]);
// Count total available products
db.products.aggregate([
    { $match: { available: true } },
    { $count: "totalAvailableProducts" }
]);
// Example: unwind tags
db.products.aggregate([
    { $unwind: "$tags" }
]);
// Example: join products with suppliers
db.products.aggregate([
    {
        $lookup: {
            from: "suppliers",
            localField: "category",
            foreignField: "category",
            as: "supplierDetails"
        }
    }
]);
// Store average price per category in new collection
db.products.aggregate([
    { $group: { _id: "$category", avgPrice: { $avg: "$price" } } },
    { $out: "category_avg_prices" }
]);
// Merge product stock summary into another collection
db.products.aggregate([
    { $group: { _id: "$category", totalStock: { $sum: "$stock" } } },
    { $merge: { into: "stock_summary" } }
]);
// Add discounted price field (10% discount)
db.products.aggregate([
    { $addFields: { discountedPrice: { $multiply: ["$price", 0.9] } } }
]);
// Replace root with name and price only
db.products.aggregate([
    { $replaceRoot: { newRoot: { name: "$name", cost: "$price" } } }
]);
// Bucket products by price range
db.products.aggregate([
    {
        $bucket: {
            groupBy: "$price",
            boundaries: [0, 1000, 5000, 20000, 50000, 60000],
            default: "Others",
            output: { count: { $sum: 1 } }
        }
    }
]);
// Run multiple aggregations at once
db.products.aggregate([
    {
        $facet: {
            "expensive": [{ $match: { price: { $gt: 20000 } } }],
            "cheapest": [{ $sort: { price: 1 } }, { $limit: 5 }],
            "totalCount": [{ $count: "totalProducts" }]
        }
    }
]);







// Index 

db.products.createIndex({ category: 1 });
db.products.createIndex({ category: 1, price: -1 });
db.products.getIndexes();
db.products.dropIndex({ category: 1 });


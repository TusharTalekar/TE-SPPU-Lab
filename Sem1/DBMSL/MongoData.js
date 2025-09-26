db.products.insertMany([
  // Electronics
  { name: "Laptop", category: "Electronics", price: 55000, stock: 25, available: true },
  { name: "Mobile", category: "Electronics", price: 20000, stock: 50, available: true },
  { name: "Tablet", category: "Electronics", price: 30000, stock: 18, available: true },
  { name: "Smartwatch", category: "Electronics", price: 12000, stock: 40, available: true },
  { name: "Earbuds", category: "Electronics", price: 4000, stock: 100, available: true },
  { name: "Camera", category: "Electronics", price: 45000, stock: 8, available: true },
  { name: "Gaming Console", category: "Electronics", price: 35000, stock: 12, available: false },
  { name: "Monitor", category: "Electronics", price: 15000, stock: 20, available: true },

  // Furniture
  { name: "Chair", category: "Furniture", price: 3000, stock: 60, available: true },
  { name: "Table", category: "Furniture", price: 7000, stock: 35, available: true },
  { name: "Sofa", category: "Furniture", price: 25000, stock: 10, available: false },
  { name: "Bed", category: "Furniture", price: 30000, stock: 15, available: true },
  { name: "Bookshelf", category: "Furniture", price: 6000, stock: 22, available: true },
  { name: "Wardrobe", category: "Furniture", price: 20000, stock: 5, available: false },

  // Clothing
  { name: "T-Shirt", category: "Clothing", price: 800, stock: 120, available: true },
  { name: "Jeans", category: "Clothing", price: 2000, stock: 75, available: true },
  { name: "Jacket", category: "Clothing", price: 3500, stock: 40, available: false },
  { name: "Dress", category: "Clothing", price: 4000, stock: 30, available: true },
  { name: "Shoes", category: "Clothing", price: 2500, stock: 55, available: true },
  { name: "Cap", category: "Clothing", price: 500, stock: 150, available: true },

  // Groceries
  { name: "Milk", category: "Groceries", price: 60, stock: 200, available: true },
  { name: "Bread", category: "Groceries", price: 40, stock: 180, available: true },
  { name: "Rice", category: "Groceries", price: 1200, stock: 90, available: true },
  { name: "Wheat Flour", category: "Groceries", price: 800, stock: 70, available: true },
  { name: "Eggs (dozen)", category: "Groceries", price: 70, stock: 150, available: true },
  { name: "Cooking Oil", category: "Groceries", price: 150, stock: 100, available: false },
  { name: "Sugar", category: "Groceries", price: 45, stock: 110, available: true },

  // Stationery
  { name: "Notebook", category: "Stationery", price: 60, stock: 300, available: true },
  { name: "Pen", category: "Stationery", price: 20, stock: 500, available: true },
  { name: "Pencil", category: "Stationery", price: 10, stock: 400, available: true },
  { name: "Eraser", category: "Stationery", price: 5, stock: 350, available: true },
  { name: "Marker", category: "Stationery", price: 50, stock: 250, available: false },
  { name: "Stapler", category: "Stationery", price: 150, stock: 100, available: true },

  // Appliances
  { name: "Refrigerator", category: "Appliances", price: 45000, stock: 10, available: true },
  { name: "Washing Machine", category: "Appliances", price: 30000, stock: 12, available: true },
  { name: "Microwave", category: "Appliances", price: 12000, stock: 20, available: true },
  { name: "Air Conditioner", category: "Appliances", price: 40000, stock: 8, available: false },
  { name: "Fan", category: "Appliances", price: 3000, stock: 35, available: true },
  { name: "Heater", category: "Appliances", price: 5000, stock: 25, available: true },

  // Sports
  { name: "Cricket Bat", category: "Sports", price: 2500, stock: 40, available: true },
  { name: "Football", category: "Sports", price: 1500, stock: 30, available: true },
  { name: "Badminton Racket", category: "Sports", price: 1200, stock: 50, available: true },
  { name: "Tennis Ball Pack", category: "Sports", price: 600, stock: 70, available: false },
  { name: "Gym Dumbbells", category: "Sports", price: 5000, stock: 15, available: true },
  { name: "Yoga Mat", category: "Sports", price: 800, stock: 80, available: true }
]);

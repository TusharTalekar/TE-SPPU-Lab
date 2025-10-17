// Map-reduces operations:Implement Map reduces operation with suitable example using MongoDB.


db.category_stock_summary.deleteMany({});


var mapFunction = function () {
    emit(this.category, this.stock);
};
var reduceFunction = function (key, values) {
    return Array.sum(values);
};
db.products.mapReduce(
    mapFunction,
    reduceFunction,
    { out: "category_stock_summary" }
);



db.products.mapReduce(
    function () {
        emit(this.category, this.stock);   // map function
    },
    function (key, values) {
        return Array.sum(values);          // reduce function
    },
    {
        out: "category_stock_summary"      // output collection
    }
);



db.category_stock_summary.find();





db.products.mapReduce(
    function () {
        emit(this.category, { stock: this.stock, price: this.price });
    },
    function (key, values) {
        var reduced = { stock: 0, price: 0, count: 0 };
        values.forEach(element => {
            reduced.stock += element.stock;
            reduced.price += element.price;
            reduced.count += 1;
        });
        return reduced;
    },
    {
        out: "category_stock_price_summary",
        finalize: function (key,reduced) {
            reduced.avgPrice = (reduced.price / reduced.count).toFixed(2);
            return reduced;
        },
    }
);


db.category_stock_price_summary.find();

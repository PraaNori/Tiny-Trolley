README FILE
# Tiny Trolly
Tiny Trolly is a mini e-commerce platform designed to showcase the
functionality of GTK (GIMP Toolkit) using Code::Blocks IDE. It
simulates an online shopping experience where users can browse and
"purchase" fashion items, furniture, electronics, and books.
## Features
- **User Interface**: Tiny Trolly provides a user-friendly
interface built using GTK, allowing for easy navigation and
interaction.
- **Product Categories**: Users can browse products across
multiple categories including fashion, furniture, electronics, and
books.
- **Product Listings**: Each category contains a list of available
products with details such as name, price, and description. It
also contains reduced price for every product with discount
included.
- **Add to Trolly**: Users can add products to their shopping cart
for later purchase.
- **Checkout**: Simulated checkout process where users can review
their selected items and finalize their order.
- **Mock Data**: Pre-loaded data using file handling to simulate a
variety of products for demonstration purposes.
- **Profile**: A profile feature which shows the user their
username, phone number, email and address when clicked on the
profile button.
- **Entry**: The basic signin and signup windows which allows the
user to sign in to our tiny trolly by entering various fields
like username, phone number, email, password and address. Later,
they can signin with their username and password where their
details would be stored beforehand.
##Implementation
- **Setup Development Environment**:
Install Code::Blocks IDE for C programming.Configure Code::Blocks
to work with GTK, ensuring that you can utilize GTK functions and
features in your project.
- **Create Project Structure**:
•Set up a project directory where you'll organize your source code
files, headers, and any other resources.Create separate files for
different aspects of your project, such as main.c for the main
application logic, product.c for product-related functions, and
ui.c for user interface functions.
- **Include GTK Library**:
• Include the necessary GTK header files in your source code files
using #include <gtk/gtk.h> directives.
•Link your project with the GTK library during compilation to
ensure that GTK functions are available for use.
- **Define Data Structures**:
• Define data structures to represent products, such as a struct
containing fields like name, price, description, and category.
•Mainly use linked lists, or other data structures to manage
collections of products.
- **Load Mock Data**:
• Create functions to load mock data into memory from predefined
sources such as text files.
- **User Interface Design**:
• Design the user interface using GTK widgets such as GtkGrid,
GtkListBox, GtkDialog, GtkButton, etc.
•Create functions to initialize and configure UI elements, and
manage user interactions.
## Requirements
- Code::Blocks IDE
- GTK (GIMP Toolkit) library
- C programming knowledge
- GTK programming knowledge
## Installation
1. Open the project in Code::Blocks.
2. Ensure that the GTK library is properly configured in your
development environment.
3. Build and run the project.

## Usage
1. Upon launching the application, users will be presented with
the main interface displaying product categories.
2. Select a category of interest to view the available products
within that category.
3. Click on a product to view its details.
4. To add a product to the cart, click the "Add to Trolly" button.
5. Navigate to the cart to review selected items by clicking on
the trolly icon.
6. Proceed to checkout to finalize the order.
7. Optionally, users can clear the cart or continue shopping.
## Contribution
- Chinthala Vikas(B23CY1004)
- Vaddi Vamsikrishna Reddy(B23CM1045)
- Jandhyam Sai Sriya(B23CS1021)
- Pradeepika Nori(B23CS1047)
## Acknowledgments
- We thank our Dr. Mayank Vatsa for giving us an opportunity to
showcase our skills.
## Disclaimer
Tiny Trolly is a fictional e-commerce platform created solely for
educational purposes. It does not facilitate actual transactions
or product sales.

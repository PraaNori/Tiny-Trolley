#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <cairo.h>
#include <stdbool.h>
#include <dirent.h>
// Structure to hold user details

typedef struct {
    gchar *name;
    gchar *phone;
    gchar *email;
    gchar *password;
    gchar *address;
} UserDetails;

GtkWidget *buynowbutton;
GtkWidget *Dscrolled_window=NULL;
GtkWidget *Dwindow;
GtkWidget *Dfixed;
GtkWidget *grid;
GtkWidget *signin_window;
GtkWidget *seller_signin_window;
GtkWidget *signin_success_window ;
GtkWidget *window;
GtkWidget *button_signin;
GtkWidget *button_signup;
GtkWidget *button_signup1;
GtkWidget *button_signin1;
GtkWidget *button_entertain;
GtkWidget *fixed;
GtkWidget *label;
GtkWidget *label3;
GtkWidget *entry;
GtkWidget *label4;
GtkWidget *search_entry;
GtkWidget *label2;
GtkWidget *scrolled_window=NULL;
gchar *return_label;
gchar *sub_cat_name;

gchar *previouscat;
gchar *permanent;
gchar *sec_return;
char *search_text;
GtkWidget *search_entry ;
gchar *currentuser;
gchar *str;
void hide_all_widgets(GtkWidget *button, gpointer data);
void return_call(GtkWidget *container, gpointer data);

// Callback function to handle the 'activate' signal of entry widgets
 void on_entry_activate(GtkEntry *entry, gpointer user_data) {
    // Get the next entry widget in the sequence
    GtkWidget *next_entry = GTK_WIDGET(user_data);

    // If next_entry is NULL, it means this is the last entry widget
    if (next_entry == NULL) {
        // Get the button widget from the user data
        GtkWidget *button = GTK_WIDGET(entry);

        // Get the parent window
        GtkWidget *window = gtk_widget_get_toplevel(GTK_WIDGET(entry));

        // Set the button as the default widget of the window
        gtk_window_set_default(GTK_WINDOW(window), button);

        // Emit the 'clicked' signal of the button
        g_signal_emit_by_name(button, "clicked");
    } else {
        // Set focus to the next entry widget
        gtk_widget_grab_focus(next_entry);
    }
}
void clear_trolley() {
    // Open the file for reading
    FILE *file = fopen("trolly.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Open a temporary file for writing
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        fclose(file);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *cuser = strtok(line, ",");
        if (strcmp(cuser, currentuser) != 0) {
            fputs(line, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);

   remove("trolly.txt");
   rename("temp.txt", "trolly.txt");

}

// Function to read user details from file
bool read_user_details(const char *filename, const gchar *username, const gchar *password) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return false;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *address = strtok(NULL, ",");

        if (strcmp(name, username) == 0 && strcmp(pass, password) == 0) {
            fclose(file);
            return true; // User found

        }
    }

    fclose(file);
    return false; // User not found
}

void profile(GtkWidget *container, gpointer data) {
    UserDetails user;
    // Cast data to GtkWidget pointer
    GtkWidget *fixed = GTK_WIDGET(data);

    // Create back button
    GtkWidget *back = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(fixed), back, 10, 20);
    gtk_widget_show(back);

    // Connect back button clicked signal to return_call function
    g_signal_connect(back, "clicked", G_CALLBACK(return_call), fixed);

    // Open user details file
    FILE *file = fopen("user_details4.workspace", "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    // Read user details from file
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, ",");
        char *phone = strtok(NULL, ",");
        char *email = strtok(NULL, ",");
        char *pass = strtok(NULL, ",");
        char *address = strtok(NULL, "\n");

        if (strcmp(name, currentuser) == 0) {
            // Update user details
            user.name = g_strdup(name);
            user.phone = g_strdup(phone);
            user.email = g_strdup(email);
            user.password = g_strdup(pass);
            user.address = g_strdup(address);

            fclose(file);

            // Concatenate user details string
            gchar *details_str = g_strdup_printf("<span font_desc='Arial Bold 20'>Name: %s\nPhone: %s\nEmail: %s\nAddress: %s\n</span>",
                                                  user.name, user.phone, user.email, user.address);

            // Create a label to display user details with markup
            GtkWidget *details_label = gtk_label_new(details_str);
            gtk_label_set_markup(GTK_LABEL(details_label), details_str);
            g_free(details_str);

            // Set line wrap mode
            gtk_label_set_line_wrap(GTK_LABEL(details_label), TRUE);

            // Add the label to the fixed container
            gtk_fixed_put(GTK_FIXED(fixed), details_label, 10, 50);
            gtk_widget_show(details_label);

            return;
        }
    }

    fclose(file);

}


typedef struct {
    char PP[100];
    char name[100];
    char img1[20];
    char img2[20];
    char img3[20];
    int MRP;
    int price;
    float rating;
    char short_des[100];
    char details[500];
    int discount;
    char offer2[50];
    struct Product *next;
} Product;

typedef struct {
    char name[100];
    Product *products;
    struct Category *next;
} Category;
Product *reqproducts;
Product *reqproducts2;
Product *offerlist;
Product *currentproduct;
Category *reqcategroy;
Category *reqcategroy2;
Category *reqcategroy3;
Product *old;
Product *neww;
Product *listfordview;
Product *listforsearch;

Category *createCategory(const char *name) {
    Category *category = (Category *)malloc(sizeof(Category));
    if (!category) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(category->name, name);
    category->products = NULL;
    category->next = NULL;
    return category;
}

Product *createProduct(const char *PP, const char *img1, const char *img2, const char *img3, const char *name, int MRP, int price, float rating, const char *short_des, const char *details, int offer1, char *offer2){
    Product *product = (Product *)malloc(sizeof(Product));
    if (!product) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy(product->PP, PP);
    strcpy(product->img1, img1);
    strcpy(product->img2,img2);
    strcpy(product->img3, img3);
    strcpy(product->offer2, offer2);

    strcpy(product->name, name);
    product->discount=offer1;
    product->MRP = MRP;
    int pricee= price-((offer1/100)*MRP);
    product->price = pricee;
    product->rating = rating;
    strcpy(product->short_des, short_des);
    strcpy(product->details, details);
    product->next = NULL;
    return product;
}

void addProductToCategory(Category *category, Product *product) {
     if (category->products != NULL) {
        old=neww;
        neww=product;
     }
     neww=product;
    if (category->products == NULL) {
        category->products = product;

    } else {
        old->next=neww;
    }
}
/*
ADD MEMORY FREE FUNCTION
KJWEBFCW
SDKJBWA
DONOT FORGET
*/

void buynow_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *container = GTK_WIDGET(data);
    gtk_widget_destroy(button);

    // Construct order confirmation message
    char str[1024];
  sprintf(str, "Order placed. Your product %s will be delivered soon.", listfordview->name);

    // Write order details to the order file
    FILE *file = fopen("order.workspace", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for writing order.\n");
        return;
    }
    fprintf(file, "%s, %s, %s, %d, %d, %.2f, %s\n", currentuser,
            listfordview->name, listfordview->img1, listfordview->MRP,
            listfordview->price, listfordview->rating, listfordview->short_des);
    fclose(file);

    // Create and display label with order confirmation message
    GtkWidget *label = gtk_label_new(str);
    gtk_fixed_put(GTK_FIXED(container), label, 100, 800);
    gtk_widget_show(label);
}
void D_view(GtkWidget *button, gpointer data) {
    gchar *str = (char *)data;
    while (reqproducts2) {
        if (strcmp(reqproducts2->name, str) == 0) {
            listfordview = reqproducts2;
            break;
        } else
            reqproducts2 = reqproducts2->next;
    }

    GtkWidget *Dwindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(Dwindow), listfordview->name);
    gtk_container_set_border_width(GTK_CONTAINER(Dwindow), 10);
    gtk_widget_set_size_request(Dwindow, 1200, 900);

    GtkWidget *Dfixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(Dwindow), Dfixed);

 GtkWidget *image = gtk_image_new_from_file("QR.jpg");
    gtk_fixed_put(GTK_FIXED(Dfixed), image, 750,100);
    // Add product name label
    GtkWidget *name_label = gtk_label_new(listfordview->name);
    gtk_widget_set_halign(name_label, GTK_ALIGN_START);
    gtk_widget_override_font(name_label, pango_font_description_from_string("Arial Bold 20")); // Increase font size
    gtk_fixed_put(GTK_FIXED(Dfixed), name_label, 10, 210);

    // Add product description label
    GtkWidget *description_label = gtk_label_new(listfordview->short_des);
    gtk_widget_set_halign(description_label, GTK_ALIGN_START);
    gtk_widget_override_font(description_label, pango_font_description_from_string("Arial 18")); // Increase font size
    gtk_fixed_put(GTK_FIXED(Dfixed), description_label, 10, 250);

    // Add MRP label
    char mrp_text[50];
    sprintf(mrp_text, "MRP: %d", listfordview->MRP);
    GtkWidget *mrp_label = gtk_label_new(mrp_text);
    gtk_widget_set_halign(mrp_label, GTK_ALIGN_START);
    gtk_widget_override_font(mrp_label, pango_font_description_from_string("Arial 14")); // Increase font size
    gtk_fixed_put(GTK_FIXED(Dfixed), mrp_label, 10, 285);

    // Add price label
    char price_text[50];
    sprintf(price_text, "Price: %d", listfordview->price);
    GtkWidget *price_label = gtk_label_new(price_text);
    gtk_widget_set_halign(price_label, GTK_ALIGN_START);
    gtk_widget_override_font(price_label, pango_font_description_from_string("Arial 14")); // Increase font size
    gtk_fixed_put(GTK_FIXED(Dfixed), price_label, 10, 315);

    // Add rating label
    char rating_text[50];
    sprintf(rating_text, "Rating: %.2f", listfordview->rating);
    GtkWidget *rating_label = gtk_label_new(rating_text);
    gtk_widget_set_halign(rating_label, GTK_ALIGN_START);
    gtk_widget_override_font(rating_label, pango_font_description_from_string("Arial 14")); // Increase font size
    gtk_fixed_put(GTK_FIXED(Dfixed), rating_label, 10, 345);

    // Add "Buy Now" button
    GtkWidget *buynow_button = gtk_button_new_with_label("Buy Now");
    gtk_widget_set_size_request(buynow_button, 200, 100);
    gtk_fixed_put(GTK_FIXED(Dfixed), buynow_button, 550, 555);
    g_signal_connect(buynow_button, "clicked", G_CALLBACK(buynow_clicked), Dfixed);

    gtk_widget_show_all(Dwindow);

    g_free(str);
}

void buy_clicked(GtkWidget *button, gpointer data) {
    // Retrieve product count and grand total
    int product_count = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "product_count"));
    float grand_total = GPOINTER_TO_INT(g_object_get_data(G_OBJECT(button), "grand_total"));

    // Construct order confirmation message
    gchar *message = g_strdup_printf("Order placed. Your total: ₹%.2f for %d product(s). Thank you!", grand_total, product_count);

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_title(GTK_WINDOW(window), "Order Confirmation");

    // Create a vertical box container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a label for the message
    GtkWidget *label = gtk_label_new(message);
gchar *message_markup = g_strdup_printf("<span font_desc='Arial Bold 20'>Order placed. Your total: ₹%.2f for %d product(s). Thank you!</span>", grand_total, product_count);
gtk_label_set_markup(GTK_LABEL(label), message_markup);
g_free(message_markup);
    gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
    gtk_widget_show(label);

    // Load and display the image
    GtkWidget *image = gtk_image_new_from_file("title.png"); // Replace "image.jpg" with the path to your image
    gtk_box_pack_start(GTK_BOX(vbox), image, TRUE, TRUE, 0);
    gtk_widget_show(image);
 // Load and display the image
    GtkWidget *image1 = gtk_image_new_from_file("logo.jpg");
    gtk_box_pack_start(GTK_BOX(vbox), image1, TRUE, TRUE, 0);
    gtk_widget_show(image1);

    // Show all widgets
    gtk_widget_show_all(window);

    // Free allocated memory
    g_free(message);
}


void clicked_trolly(GtkWidget *container, gpointer data) {
    // Cast data to GtkWidget pointer
    GtkWidget *fixed = GTK_WIDGET(data);
    Product p;

    // Create back button
    GtkWidget *back = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(fixed), back, 10, 20);
    gtk_widget_show(back);

    // Connect back button clicked signal to return_call function
    g_signal_connect(back, "clicked", G_CALLBACK(return_call), fixed);

    GtkWidget *clear = gtk_button_new_with_label("Empty trolley");
    gtk_fixed_put(GTK_FIXED(fixed), clear, 1700, 20);
    gtk_widget_show(clear);
    g_signal_connect(clear, "clicked", G_CALLBACK(clear_trolley), fixed);

    // Open user details file
    FILE *file = fopen("trolly.txt", "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    int y_offset = 40; // Initial vertical position
    float grand_total = 0;
    int product_count = 0; // Total number of products

    char *cuser;
    char *cname;
    char *cimg;
    int cmrp;
    int crate;
    float crating;
    char *cdes;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line
        cuser = strtok(line, ",");
        cname = strtok(NULL, ",");
        cimg = strtok(NULL, ",");
        cmrp = atoi(strtok(NULL, ","));
        crate = atoi(strtok(NULL, ","));
        crating = atof(strtok(NULL, ","));
        cdes = strtok(NULL, "\n");

        if (strcmp(cuser, currentuser) == 0) {
            product_count++;

            // Assign values to Product structure fields
            strcpy(p.name, cname);
            strcpy(p.img1, cimg);
            p.MRP = cmrp;
            p.price = crate;
            p.rating = crating;
            strcpy(p.short_des, cdes);

            grand_total += p.price;

            // Load the image
            GtkWidget *image = gtk_image_new_from_file(p.img1);
            gtk_fixed_put(GTK_FIXED(fixed), image, 20, y_offset);
            gtk_widget_show(image);

            // Display product details
            gchar *details_str = g_strdup_printf("<span font_desc='Arial Bold 16'>Name: %s \nMRP: %d \nFinal Price: %d \nRating: %.2f \nDescription: %s\n\n\n</span>",
                                                   p.name, p.MRP, p.price, p.rating, p.short_des);

            // Create a label to display product details with markup
            GtkWidget *details_label = gtk_label_new(NULL);
            gtk_label_set_markup(GTK_LABEL(details_label), details_str);
            g_free(details_str);

            // Set line wrap mode
            gtk_label_set_line_wrap(GTK_LABEL(details_label), FALSE);

            // Add the label to the fixed container
            gtk_fixed_put(GTK_FIXED(fixed), details_label, 100, y_offset);

            y_offset += 400;

            gtk_widget_show(details_label);
        }
    }

    fclose(file);
        // Display grand total and total number of products
    GtkWidget *total_label = gtk_label_new(NULL);
    PangoFontDescription *font_desc = pango_font_description_new();
    pango_font_description_set_family(font_desc, "Arial");
    pango_font_description_set_size(font_desc, 20* PANGO_SCALE);
    pango_font_description_set_weight(font_desc, PANGO_WEIGHT_BOLD);
    gtk_widget_modify_font(total_label, font_desc);
    pango_font_description_free(font_desc);

    gchar *label_text = g_strdup_printf("number of Products: %d \n\n Grand Total: ₹%.2f", product_count, grand_total);
    gtk_label_set_text(GTK_LABEL(total_label), label_text);
    g_free(label_text);

    // Add the label to the fixed container
    gtk_fixed_put(GTK_FIXED(fixed), total_label, 1300, 20);
    gtk_widget_show(total_label);

    GtkWidget *buy_button = gtk_button_new_with_label("Proceed to buy");
    gtk_fixed_put(GTK_FIXED(fixed), buy_button, 1500, 800);
    gtk_widget_show(buy_button);

    g_signal_connect(buy_button, "clicked", G_CALLBACK(buy_clicked), fixed);

    // Store product count and grand total for later use in buy now function
    g_object_set_data(G_OBJECT(buy_button), "product_count", GINT_TO_POINTER(product_count));
    g_object_set_data(G_OBJECT(buy_button), "grand_total", GINT_TO_POINTER(grand_total));
}


void hide_all_widgets(GtkWidget *button, gpointer data);

void Fcategories(GtkWidget *container, gpointer data);
void Ecategories(GtkWidget *container, gpointer data);
void furniture(GtkWidget *container, gpointer data);
void phones(GtkWidget *container, gpointer data);
void Apple(GtkWidget *container, gpointer data);
void books(GtkWidget *container, gpointer data);


void seller(GtkWidget *container, gpointer data){



}

void return_call(GtkWidget *container, gpointer data) {
   GtkWidget *fixed = GTK_WIDGET(data);
GList *children = gtk_container_get_children(GTK_CONTAINER(fixed));
gboolean stop_destruction = FALSE;
while (children != NULL) {
    GtkWidget *widget = GTK_WIDGET(children->data);
    if (GTK_IS_BUTTON(widget)) {
        const gchar *label = gtk_button_get_label(GTK_BUTTON(widget));
        if (g_strcmp0(label, return_label) == 0) {
            stop_destruction = TRUE;
        } else if (stop_destruction) {
            gtk_widget_destroy(widget);
        }
    } else if (GTK_IS_IMAGE(widget) || GTK_IS_BOX(widget)|| GTK_IS_LABEL(widget)) {
        if (stop_destruction) {
            if (GTK_IS_BOX(widget)) {
                GList *box_children = gtk_container_get_children(GTK_CONTAINER(widget));
                while (box_children != NULL) {
                    GtkWidget *box_child = GTK_WIDGET(box_children->data);
                    gtk_widget_destroy(box_child);
                    box_children = g_list_next(box_children);
                }
                g_list_free(box_children);
            }
            gtk_widget_destroy(widget);
        }
    }
    children = g_list_next(children);
}
g_list_free(children);
search_text="\0";
gtk_widget_show_all(window);
}

void add_to_trolley(GtkWidget *button, gpointer data) {
    // Cast gpointer to char* to retrieve the product name
    char *product_name = (char *)data;

    FILE *file = fopen("trolly.txt", "a");
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return;
    }

    fprintf(file,"%s, %s, %s, %d, %d, %.2f, %s\n", currentuser,
            product_name, reqproducts2->PP,
            reqproducts2->MRP, reqproducts2->price, reqproducts2->rating, reqproducts2->short_des);


    fclose(file);

    printf("Product '%s' added to trolley.\n", product_name);
}void order(GtkWidget *container, gpointer data) {
    GtkWidget *fixed = GTK_WIDGET(data);
    GtkWidget *back = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(fixed), back, 10, 20);
    gtk_widget_show(back);

    g_signal_connect(back, "clicked", G_CALLBACK(return_call), fixed);
    FILE *file = fopen("order.workspace", "r");
    if (file == NULL) {
        return;
    }
    char *cuser;
    char *cname;
    char *cimg;
    int cmrp;
    int crate;
    float crating;
    char *cdes;

    char line[256];
    int y_offset = 50; // Initial vertical position
    int label_height = 0; // Height of the last label
    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line
        cuser = strtok(line, ",");
        cname = strtok(NULL, ",");
        cimg = strtok(NULL, ",");
        cmrp = atoi(strtok(NULL, ","));
        crate = atoi(strtok(NULL, ","));
        crating = atof(strtok(NULL, ","));
        cdes = strtok(NULL, ",");

        // Display order details
        gchar *order_str = g_strdup_printf("<span font_desc='Arial Bold 20'>Name: %s\nMRP: %d\nFinal Price: %d\nRating: %.2f\nDescription: %s\n\n\n</span>",
                                           cname, cmrp, crate, crating, cdes);
        // Create a label to display order details with markup
        GtkWidget *order_label = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(order_label), order_str);
        g_free(order_str);

        // Set line wrap mode
        gtk_label_set_line_wrap(GTK_LABEL(order_label), TRUE);

        // Add the label to the fixed container
        gtk_fixed_put(GTK_FIXED(fixed), order_label, 10, y_offset + label_height); // Adjust position as needed

        // Get the height of the label
        gtk_widget_get_size_request(order_label, NULL, &label_height);

        // Increment vertical position for next label
        y_offset=y_offset+500;
        // Show the label
        gtk_widget_show(order_label);
    }

    // Close the file
    fclose(file);
}

void initiate_search(GtkWidget *entry, gpointer data) {
    search_text = gtk_entry_get_text(GTK_ENTRY(entry));

}

int check(const char *s1, const char *s2){
   int len1 = strlen(s1);
    int len2 = strlen(s2);
int c=0;
for(int i=0;i<len1;i++){
if(s1[i]==s2[i]){
   c++;
}
}
int d=len1/5;

  if (len1-c<d) return 1;
  else return 0;
}
int present_or_absent(char *actual_product,char *searched_product){
char *token;
    char copy[1000];
    strcpy(copy, actual_product);

    char words[1000];
    strcpy(words, searched_product);

    token = strtok(words, " ");

    while (token != NULL) {
        char sentence_copy[1000];
        strcpy(sentence_copy, copy);
        char *sentence_token = strtok(sentence_copy, " ");

        int present = 0;

        while (sentence_token != NULL) {
            if (strcmp(sentence_token, token) == 0) {
                present = 1;
                break;
            }

            if (check(sentence_token, token)) {

                present = 1;
                break;
            }
            sentence_token = strtok(NULL, " ");
        }

        if (!present) {
            return 0;
        }

        token = strtok(NULL, " ");
    }

    return 1;

}

void searching(){
GtkWidget *containerr=GTK_WIDGET(fixed);
GtkWidget *button2 = gtk_button_new_with_label("  X  ");
    gtk_fixed_put(GTK_FIXED(containerr), button2, 30, 30);
    gtk_widget_show(button2);
     g_signal_connect(button2, "clicked", G_CALLBACK(hide_all_widgets), containerr);
int v=0,h=1,h_shift=0,v_shift=0;

    g_print("%s",search_text);
 while (reqcategroy2) {
    listforsearch = reqcategroy2->products;
    reqproducts2=listforsearch;
    while (listforsearch) {
        if (present_or_absent(listforsearch->name,search_text)) {
                 if((h%2)==0){
    h_shift=915;
 }else {
     h_shift=0;
 if(h>2){
v_shift=v_shift+370;
 }
 }

gchar *labelb = g_strdup_printf(listforsearch->img1);
GtkWidget *button = gtk_button_new_with_label(labelb);
gtk_widget_set_size_request(button, 900, 350);
g_free(labelb);
gtk_fixed_put(GTK_FIXED(containerr), button,h_shift+ 10,v_shift+ 550);
gtk_widget_show(button);

gchar *labelp = g_strdup_printf(listforsearch->PP);
GtkWidget *fasimg = gtk_image_new_from_file(labelp);
gtk_fixed_put(GTK_FIXED(containerr), fasimg,h_shift+ 15,v_shift+ 555);
gtk_widget_show(fasimg);

GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Vertical box for stacking labels vertically
gtk_fixed_put(GTK_FIXED(containerr), box,h_shift+ 530,v_shift+ 590);
gtk_widget_set_size_request(box, 370, 320);

GtkWidget *label = gtk_label_new(listforsearch->name);
PangoFontDescription *font_desc = pango_font_description_from_string("Arial Bold 15");
gtk_widget_override_font(label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(label, GTK_ALIGN_START);
gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
gtk_container_add(GTK_CONTAINER(box), label);

GtkWidget *mrp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), mrp_box);

// Create the label for displaying "MRP:"
GtkWidget *mrp_label = gtk_label_new("MRP: ");
gtk_widget_set_halign(mrp_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_label);

// Create the label for displaying the MRP value
char MRP_str[7];
sprintf(MRP_str, "%d", listforsearch->MRP);
strcat(MRP_str,"  ");

GtkWidget *mrp_value_label = gtk_label_new(MRP_str);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(mrp_value_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(mrp_value_label, GTK_ALIGN_START);
PangoAttrList *strike_attr_list = pango_attr_list_new();
PangoAttribute *strike_attr = pango_attr_strikethrough_new(TRUE);
pango_attr_list_insert(strike_attr_list, strike_attr);
gtk_label_set_attributes(GTK_LABEL(mrp_value_label), strike_attr_list);
pango_attr_list_unref(strike_attr_list);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_value_label);

char price_str[7];
sprintf(price_str, "%d", listforsearch->price);
strcat(price_str,"  ");
GtkWidget *price_label = gtk_label_new(price_str);
font_desc = pango_font_description_from_string("Arial bold 20");
gtk_widget_override_font(price_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(price_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), price_label);

char discount[7];
int temp = listforsearch->MRP - listforsearch->price;
temp=temp*100;
temp=(temp/listforsearch->MRP);
sprintf(discount, "%d",temp);
strcat(discount,"% off");
GtkWidget *dis_label = gtk_label_new(discount);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(dis_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(dis_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), dis_label);

GtkWidget *ratings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), ratings_box);

GtkWidget *ratings_label = gtk_label_new("Rating: ");
gtk_widget_set_halign(ratings_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), ratings_label);

char rating_str[7];
sprintf(rating_str, "%.2f", listforsearch->rating);
GtkWidget *rating_label = gtk_label_new(rating_str);
font_desc = pango_font_description_from_string("Arial bold 12");
gtk_widget_override_font(rating_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(rating_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), rating_label);

GtkWidget *buttom = gtk_button_new_with_label("Add to Trolley");
gtk_widget_set_size_request(buttom,300 , 30);
gtk_fixed_put(GTK_FIXED(containerr), buttom,h_shift+ 550,v_shift+ 340);
gtk_widget_show(buttom);

char *str = g_strdup(listforsearch->name);
printf("%s\n\n",str);
g_signal_connect(button, "clicked", G_CALLBACK(D_view), str);
g_signal_connect(buttom, "clicked", G_CALLBACK(add_to_trolley), str);

gtk_widget_show_all(box);

 h=h+1;
        }
        listforsearch=listforsearch->next;
    }reqcategroy2=reqcategroy2->next;
 }


}


void products(const gchar* f,gpointer data) {
     GtkWidget *containerr = GTK_WIDGET(data);
     gtk_container_add(GTK_CONTAINER(window), containerr);
     GtkWidget *bottom = gtk_button_new_with_label("<<<--");
     gtk_fixed_put(GTK_FIXED(containerr), bottom, 10, 10);
     gtk_widget_show(bottom);
     g_signal_connect(bottom, "clicked", G_CALLBACK(hide_all_widgets), containerr);



    gchar *bname = sub_cat_name;
    gchar *dot_position = strchr(bname, '.');
    if (dot_position != NULL) {
        *dot_position = '\0';
    }

while (reqcategroy) {
    if (strcmp(reqcategroy->name, sub_cat_name) == 0) {
        reqproducts = reqcategroy->products;
        break;
    }
    reqcategroy = reqcategroy->next;
}
reqproducts2=reqproducts;
int v=0,h=1,h_shift=0,v_shift=0;

while(reqproducts){

 if((h%2)==0){
    h_shift=915;
 }else {
     h_shift=0;
 if(h>2){
v_shift=v_shift+370;
 }
 }

gchar *labelb = g_strdup_printf(reqproducts->img1);
GtkWidget *button = gtk_button_new_with_label("img");
gtk_widget_set_size_request(button, 900, 350);
g_free(labelb);
gtk_fixed_put(GTK_FIXED(containerr), button,h_shift+ 10,v_shift+ 50);
gtk_widget_show(button);




gchar *labelp = g_strdup_printf(reqproducts->PP);
GtkWidget *fasimg = gtk_image_new_from_file(labelp);
gtk_fixed_put(GTK_FIXED(containerr), fasimg,h_shift+ 15,v_shift+ 55);
gtk_widget_show(fasimg);

// Create a box
GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Vertical box for stacking labels vertically
gtk_fixed_put(GTK_FIXED(containerr), box,h_shift+ 530,v_shift+ 70);
gtk_widget_set_size_request(box, 370, 320);

// Create the first label for product name
GtkWidget *label = gtk_label_new(reqproducts->name);
PangoFontDescription *font_desc = pango_font_description_from_string("Arial Bold 15");
gtk_widget_override_font(label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(label, GTK_ALIGN_START);
gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
gtk_container_add(GTK_CONTAINER(box), label);

GtkWidget *description = gtk_label_new(reqproducts->short_des);
font_desc = pango_font_description_from_string("Arial Bold 13");
gtk_widget_override_font(description, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(description, GTK_ALIGN_START);
gtk_label_set_line_wrap(GTK_LABEL(description), TRUE);
gtk_label_set_line_wrap_mode(GTK_LABEL(description), PANGO_WRAP_WORD_CHAR);
gtk_container_add(GTK_CONTAINER(box), description);


// Create a box for holding MRP label and its corresponding value
GtkWidget *mrp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), mrp_box);

// Create the label for displaying "MRP:"
GtkWidget *mrp_label = gtk_label_new("MRP: ");
gtk_widget_set_halign(mrp_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_label);

// Create the label for displaying the MRP value
char MRP_str[7];
sprintf(MRP_str, "%d", reqproducts->MRP);
strcat(MRP_str,"  ");

GtkWidget *mrp_value_label = gtk_label_new(MRP_str);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(mrp_value_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(mrp_value_label, GTK_ALIGN_START);
PangoAttrList *strike_attr_list = pango_attr_list_new();
PangoAttribute *strike_attr = pango_attr_strikethrough_new(TRUE);
pango_attr_list_insert(strike_attr_list, strike_attr);
gtk_label_set_attributes(GTK_LABEL(mrp_value_label), strike_attr_list);
pango_attr_list_unref(strike_attr_list);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_value_label);

char price_str[7];
sprintf(price_str, "%d", reqproducts->price);
strcat(price_str,"  ");
GtkWidget *price_label = gtk_label_new(price_str);
font_desc = pango_font_description_from_string("Arial bold 20");
gtk_widget_override_font(price_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(price_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), price_label);

char discount[7];
int temp = reqproducts->MRP - reqproducts->price;
temp=temp*100;
temp=(temp/reqproducts->MRP);
sprintf(discount, "%d",temp);
strcat(discount,"% off");
GtkWidget *dis_label = gtk_label_new(discount);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(dis_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(dis_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), dis_label);

GtkWidget *ratings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), ratings_box);

GtkWidget *ratings_label = gtk_label_new("Rating: ");
gtk_widget_set_halign(ratings_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), ratings_label);

char rating_str[7];
sprintf(rating_str, "%.2f", reqproducts->rating);
GtkWidget *rating_label = gtk_label_new(rating_str);
font_desc = pango_font_description_from_string("Arial bold 12");
gtk_widget_override_font(rating_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(rating_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), rating_label);

GtkWidget *buttom = gtk_button_new_with_label("Add to Trolley");
gtk_widget_set_size_request(buttom,300 , 30);
gtk_fixed_put(GTK_FIXED(containerr), buttom,h_shift+ 550,v_shift+ 340);
gtk_widget_show(buttom);
str = g_strdup(reqproducts->name);
printf("%s\n",reqproducts->offer2);
g_signal_connect(buttom, "clicked", G_CALLBACK(add_to_trolley), str);
g_signal_connect(button, "clicked", G_CALLBACK(D_view), str);

gtk_widget_show_all(box);
reqproducts=reqproducts->next;

 h=h+1;

}


}



void viewproducts(const char *folder) {
    GtkWidget *containerr = GTK_WIDGET(fixed);
     gtk_container_add(GTK_CONTAINER(window), containerr);

    DIR *d;
    struct dirent *dir;
    d = opendir(folder);
    Category *head = NULL;
    g_print("h\n");
    if (d) {
        Category *currentCategory = NULL;
        while ((dir = readdir(d)) != NULL) {
            FILE *file;
            char filename[100];
            sprintf(filename, "%s/%s", folder, dir->d_name);
            file = fopen(filename, "r");
            if (file) {
                char line[1024];
                char currentPP[100] = {0};
                char currentProductName[100] = {0};
                int currentMRP = 0;
                int currentPrice = 0;
                float currentRating = 0.0f;
                char currentShortDes[100] = {0};
                int isReadingProduct = 0;
                char currdetails[500] = {0};
                char img1[20] = {0};
                char img2[20] = {0};
                char img3[20] = {0};
                int offer1 = 0;
                char offer2[50] = {0};
                while (fgets(line, sizeof(line), file)) {
             //       line[strcspn(line, "\n")] = 0; // Remove newline character
                    if (line[0] == '[') { // Start new category
                        Category *newnode = createCategory(line);
                        if (!head) {
                            head = newnode;
                            currentCategory = head;
                        } else {
                            currentCategory->next = newnode;
                            currentCategory = newnode;
                        }
                    } else if (line[0] == ']') { // End current category
                        isReadingProduct = 0;
                    } else if (line[0] == ';') { // End current product
                        Product *newProduct = createProduct(currentPP, img1, img2, img3, currentProductName, currentMRP, currentPrice, currentRating, currentShortDes, currdetails, offer1, offer2);
                        addProductToCategory(currentCategory, newProduct);
                        isReadingProduct = 0; // Reset product reading state
                    } else if (isReadingProduct) {
                        // Parse additional product details if needed
                        if (strstr(line, "|")) {  // Check if line contains product details
                            sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%f|%[^|]|%[^|]|%d|%[^|]", img1, img2, img3, currentProductName, &currentMRP, &currentPrice, &currentRating, currentShortDes, currdetails, &offer1, offer2);
                        }
                    } else {
                        // It's the product PP (Product Code)
                        strcpy(currentPP, line);
                        isReadingProduct = 1; // Next lines are part of this product until ';' encountered
                    }
                }
                fclose(file);
            }
        }
        closedir(d); // Close the directory
    } else {
        printf("Error opening directory.\n");
    }
reqcategroy2=head;
reqcategroy3=head;

int v=0,h=1,h_shift=0,v_shift=0;

   while (head) {
    reqproducts2 = head->products;
    offerlist=reqproducts2;
    while (offerlist) {
        if (offerlist->discount>10) {
               if((h%2)==0){
    h_shift=915;
 }else {
     h_shift=0;
 if(h>2){
v_shift=v_shift+370;
 }
 }

gchar *labelp = g_strdup_printf(offerlist->PP);
GtkWidget *fasimg = gtk_button_new_with_label(labelp);
gtk_widget_set_size_request(fasimg, 900, 350);

gtk_fixed_put(GTK_FIXED(containerr), fasimg,h_shift+ 10,v_shift+ 550);
gtk_widget_show(fasimg);
g_free(labelp);

gchar *labelb = g_strdup_printf(offerlist->img1);
GtkWidget *button = gtk_image_new_from_file(labelb);
gtk_widget_set_size_request(button, 550, 350);
gtk_fixed_put(GTK_FIXED(containerr), button,h_shift+ 5,v_shift+ 550);
gtk_widget_show(button);


GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Vertical box for stacking labels vertically
gtk_fixed_put(GTK_FIXED(containerr), box,h_shift+ 530,v_shift+ 590);
gtk_widget_set_size_request(box, 370, 320);

GtkWidget *label = gtk_label_new(offerlist->name);
PangoFontDescription *font_desc = pango_font_description_from_string("Arial Bold 15");
gtk_widget_override_font(label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(label, GTK_ALIGN_START);
gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
gtk_label_set_line_wrap_mode(GTK_LABEL(label), PANGO_WRAP_WORD_CHAR);
gtk_container_add(GTK_CONTAINER(box), label);

GtkWidget *mrp_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), mrp_box);

// Create the label for displaying "MRP:"
GtkWidget *mrp_label = gtk_label_new("MRP: ");
gtk_widget_set_halign(mrp_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_label);

// Create the label for displaying the MRP value
char MRP_str[7];
sprintf(MRP_str, "%d", offerlist->MRP);
strcat(MRP_str,"  ");

GtkWidget *mrp_value_label = gtk_label_new(MRP_str);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(mrp_value_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(mrp_value_label, GTK_ALIGN_START);
PangoAttrList *strike_attr_list = pango_attr_list_new();
PangoAttribute *strike_attr = pango_attr_strikethrough_new(TRUE);
pango_attr_list_insert(strike_attr_list, strike_attr);
gtk_label_set_attributes(GTK_LABEL(mrp_value_label), strike_attr_list);
pango_attr_list_unref(strike_attr_list);
gtk_container_add(GTK_CONTAINER(mrp_box), mrp_value_label);

char price_str[7];
sprintf(price_str, "%d", offerlist->price);
strcat(price_str,"  ");
GtkWidget *price_label = gtk_label_new(price_str);
font_desc = pango_font_description_from_string("Arial bold 20");
gtk_widget_override_font(price_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(price_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), price_label);

char discount[7];
int temp = offerlist->MRP - offerlist->price;
temp=temp*100;
temp=(temp/offerlist->MRP);
sprintf(discount, "%d",temp);
strcat(discount,"% off");
GtkWidget *dis_label = gtk_label_new(discount);
font_desc = pango_font_description_from_string("Arial 10");
gtk_widget_override_font(dis_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(dis_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(mrp_box), dis_label);

GtkWidget *ratings_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Horizontal box for MRP label and value
gtk_container_add(GTK_CONTAINER(box), ratings_box);

GtkWidget *ratings_label = gtk_label_new("Rating: ");
gtk_widget_set_halign(ratings_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), ratings_label);

char rating_str[7];
sprintf(rating_str, "%.2f", offerlist->rating);
GtkWidget *rating_label = gtk_label_new(rating_str);
font_desc = pango_font_description_from_string("Arial bold 12");
gtk_widget_override_font(rating_label, font_desc);
pango_font_description_free(font_desc);
gtk_widget_set_halign(rating_label, GTK_ALIGN_START);
gtk_container_add(GTK_CONTAINER(ratings_box), rating_label);

GtkWidget *buttom = gtk_button_new_with_label("Add to Trolley");
gtk_widget_set_size_request(buttom,300 , 30);
gtk_fixed_put(GTK_FIXED(containerr), buttom,h_shift+ 550,v_shift+ 760);
gtk_widget_show(buttom);

char *str = g_strdup(offerlist->name);
g_signal_connect(button, "clicked", G_CALLBACK(D_view), str);
g_signal_connect(buttom, "clicked", G_CALLBACK(add_to_trolley), str);


gtk_widget_show_all(window);

 h=h+1;

        }
        offerlist = offerlist->next;
    }
    head = head->next;
}
    g_print("h\n");


}



void sub_category(GtkWidget *container, gpointer data) {

    // Load the image for the "Fashion" category
    GtkWidget *wall= gtk_image_new_from_file("wallpaper.jpg");
    gtk_fixed_put(GTK_FIXED(container), wall, 0, 0); // Adjust position as needed
    gtk_widget_show(wall);

    GtkWidget *Sbuttonn = gtk_button_new_with_label("<<---");
    gtk_fixed_put(GTK_FIXED(container), Sbuttonn, 27, 30);
    gtk_widget_show(Sbuttonn);

 // Load the image for the "Fashion" category

    // Connect signal handlers
    GtkWidget *bacimg = gtk_image_new_from_file("back.jpg");
    gtk_fixed_put(GTK_FIXED(container), bacimg, 10, 20);
    g_signal_connect(Sbuttonn, "clicked", G_CALLBACK(hide_all_widgets), container);

   FILE *file = fopen(permanent, "r");
    if (file == NULL) {
        return 1;
    }
                    char line[1024];

   Category *head = NULL, *currentCategory = NULL;
    char currentPP[100] = {0};
    char currentProductName[100] = {0};
    int currentMRP = 0;
    int currentPrice = 0;
    float currentRating = 0.0f;
    char currentShortDes[100] = {0};
    int isReadingProduct = 0;
    char currdetails[500]={0};
    char img1[20]={0};
    char img2[20]={0};
    char img3[20]={0};
    int offer1=0;
    char offer2[50]={0};
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        if (line[0] == '[') { // Start new category
            sscanf(line, "[%[^]]", line);
            currentCategory = createCategory(line);
            if (!head) {
                head = currentCategory;
            } else {
                Category *temp = head;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = currentCategory;
            }
        } else if (line[0] == ']') { // End current category
            isReadingProduct = 0;
        } else if (line[0] == ';') { // End current product
            Product *newProduct = createProduct(currentPP, img1, img2, img3, currentProductName, currentMRP, currentPrice, currentRating, currentShortDes, currdetails, offer1, offer2);

            addProductToCategory(currentCategory, newProduct);
            isReadingProduct = 0; // Reset product reading state
        } else if (isReadingProduct) {
            // Parse additional product details if needed
            if (strstr(line, "|")) {  // Check if line contains product details
                sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%f|%[^|]|%[^|]|%d|%[^|]",img1,img2,img3,currentProductName, &currentMRP, &currentPrice, &currentRating, currentShortDes,currdetails,&offer1,offer2);
            }
        } else {
            // It's the product PP (Product Code)
            strcpy(currentPP, line);
            isReadingProduct = 1; // Next lines are part of this product until ';' encountered
        }
    }
    reqcategroy=head;

    fclose(file);
int movee=0;
   while (head) {
    gchar *label = g_strdup_printf(head->name);
    GtkWidget *button = gtk_button_new_with_label(label);
    strcat(label,".jpg");
    GtkWidget *bimage = gtk_image_new_from_file(label); // Assuming head->name contains the file path
    gtk_fixed_put(GTK_FIXED(container), button, movee + 100, 200); // Position the button
    g_free(label);
gtk_widget_show(button);
gtk_widget_show(bimage);
    g_signal_connect(button, "clicked", G_CALLBACK(hide_all_widgets), container);

    movee = movee + 150;
    head = head->next;
   }

}

void Fcategories(GtkWidget *container, gpointer data) {

     // Load the image for the "Fashion" category
    GtkWidget *wall= gtk_image_new_from_file("wallpaper.jpg");
    gtk_fixed_put(GTK_FIXED(container), wall, 0, 0); // Adjust position as needed
    gtk_widget_show(wall);

         // Load the image for the "Fashion" category
    GtkWidget *bedroom_logo= gtk_image_new_from_file("kids_logo.png");
    gtk_fixed_put(GTK_FIXED(container), bedroom_logo, 50, 600); // Adjust position as needed
    gtk_widget_show(bedroom_logo);
            // Load the image for the "Fashion" category
    GtkWidget *livingroom_logo= gtk_image_new_from_file("women_logo.jpg");
    gtk_fixed_put(GTK_FIXED(container), livingroom_logo, 700, 50); // Adjust position as needed
    gtk_widget_show(livingroom_logo);
            // Load the image for the "Fashion" category
    GtkWidget *kitchen_logo= gtk_image_new_from_file("Mens_logo.jpg");
    gtk_fixed_put(GTK_FIXED(container), kitchen_logo, 1000, 600); // Adjust position as needed
    gtk_widget_show(kitchen_logo);

   GtkWidget *back = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(container), back, 30, 30);
    gtk_widget_show(back);

    GtkWidget *F1= gtk_button_new_with_label("Men");
    gtk_fixed_put(GTK_FIXED(container), F1,1250, 870);
    gtk_widget_show(F1);

    GtkWidget *F2 = gtk_button_new_with_label("Women");
    gtk_fixed_put(GTK_FIXED(container), F2, 1000, 350 );
    gtk_widget_show(F2);

    GtkWidget *F3 = gtk_button_new_with_label("Kids");
    gtk_fixed_put(GTK_FIXED(container), F3, 200, 900);
    gtk_widget_show(F3);


 g_signal_connect(F3, "clicked", G_CALLBACK(hide_all_widgets), container);
  g_signal_connect(F1, "clicked", G_CALLBACK(hide_all_widgets), container);
g_signal_connect(F2, "clicked", G_CALLBACK(hide_all_widgets), container);
  g_signal_connect(back, "clicked", G_CALLBACK(return_call), container);
}
void Ecategories(GtkWidget *container, gpointer data) {


     // Load the image for the "Fashion" category
    GtkWidget *wall= gtk_image_new_from_file("wallpaper.jpg");
    gtk_fixed_put(GTK_FIXED(container), wall, 0, 0); // Adjust position as needed
    gtk_widget_show(wall);

       // Load the image for the "Fashion" category
    GtkWidget *phones_logo= gtk_image_new_from_file("Phones_logo.png");
    gtk_fixed_put(GTK_FIXED(container), phones_logo, 100, 500); // Adjust position as needed
    gtk_widget_show(phones_logo);
       // Load the image for the "Fashion" category
    GtkWidget *Laptops_logo= gtk_image_new_from_file("Laptop_logo.jpg");
    gtk_fixed_put(GTK_FIXED(container), Laptops_logo, 700, 100); // Adjust position as needed
    gtk_widget_show(Laptops_logo);
       // Load the image for the "Fashion" category
    GtkWidget *TV_logo= gtk_image_new_from_file("TV_logo.png");
    gtk_fixed_put(GTK_FIXED(container), TV_logo, 1000, 600); // Adjust position as needed
    gtk_widget_show(TV_logo);

    GtkWidget *button2 = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(container), button2, 30, 30);
    gtk_widget_show(button2);

    GtkWidget *button1 = gtk_button_new_with_label("phones");
    gtk_fixed_put(GTK_FIXED(container), button1, 100, 250);
    gtk_widget_show(button1);

    GtkWidget *button3 = gtk_button_new_with_label("Laptops");
    gtk_fixed_put(GTK_FIXED(container), button3, 950, 220);
    gtk_widget_show(button3);

    GtkWidget *button4 = gtk_button_new_with_label("TV");
    gtk_fixed_put(GTK_FIXED(container), button4, 1200, 950);
    gtk_widget_show(button4);
 // Load the image for the "Fashion" category

    g_signal_connect(button3, "clicked", G_CALLBACK(hide_all_widgets), container);
  g_signal_connect(button1, "clicked", G_CALLBACK(hide_all_widgets), container);
  g_signal_connect(button4, "clicked", G_CALLBACK(hide_all_widgets), container);

    g_signal_connect(button2, "clicked", G_CALLBACK(return_call), container);
}

void furniture(GtkWidget *container, gpointer data) {


     // Load the image for the "Fashion" category
    GtkWidget *wall= gtk_image_new_from_file("wallpaper.jpg");
    gtk_fixed_put(GTK_FIXED(container), wall, 0, 0); // Adjust position as needed
    gtk_widget_show(wall);

         // Load the image for the "Fashion" category
    GtkWidget *bedroom_logo= gtk_image_new_from_file("bedroom.jpg");
    gtk_fixed_put(GTK_FIXED(container), bedroom_logo, 50, 600); // Adjust position as needed
    gtk_widget_show(bedroom_logo);
            // Load the image for the "Fashion" category
    GtkWidget *livingroom_logo= gtk_image_new_from_file("living room.jpg");
    gtk_fixed_put(GTK_FIXED(container), livingroom_logo, 700, 50); // Adjust position as needed
    gtk_widget_show(livingroom_logo);
            // Load the image for the "Fashion" category
    GtkWidget *kitchen_logo= gtk_image_new_from_file("kitchen.jpg");
    gtk_fixed_put(GTK_FIXED(container), kitchen_logo, 1000, 600); // Adjust position as needed
    gtk_widget_show(kitchen_logo);

   GtkWidget *back = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(container), back, 30, 30);
    gtk_widget_show(back);

    GtkWidget *F1= gtk_button_new_with_label("Bedroom");
    gtk_fixed_put(GTK_FIXED(container), F1, 200, 900);
    gtk_widget_show(F1);
    g_signal_connect(F1, "clicked", G_CALLBACK(hide_all_widgets), container);

    GtkWidget *F2 = gtk_button_new_with_label("Kitchen");
    gtk_fixed_put(GTK_FIXED(container), F2, 1250, 870);
    gtk_widget_show(F2);
 g_signal_connect(F2, "clicked", G_CALLBACK(hide_all_widgets), container);
    GtkWidget *F3 = gtk_button_new_with_label("Living Room");
    gtk_fixed_put(GTK_FIXED(container), F3, 1000, 350);
    gtk_widget_show(F3);

      g_signal_connect(back, "clicked", G_CALLBACK(return_call), container);
 g_signal_connect(F3, "clicked", G_CALLBACK(hide_all_widgets), container);
}

void books(GtkWidget *container, gpointer data) {

     // Load the image for the "Fashion" category
    GtkWidget *wall= gtk_image_new_from_file("wallpaper.jpg");
    gtk_fixed_put(GTK_FIXED(container), wall, 0, 0); // Adjust position as needed
    gtk_widget_show(wall);

         // Load the image for the "Fashion" category
    GtkWidget *Fiction_logo= gtk_image_new_from_file("fiction.jpg");
    gtk_fixed_put(GTK_FIXED(container), Fiction_logo, 50, 500); // Adjust position as needed
    gtk_widget_show(Fiction_logo);
            // Load the image for the "Fashion" category
    GtkWidget *Nonfiction_logo= gtk_image_new_from_file("Non fiction.jpg");
    gtk_fixed_put(GTK_FIXED(container), Nonfiction_logo, 500, 50); // Adjust position as needed
    gtk_widget_show(Nonfiction_logo);
            // Load the image for the "Fashion" category
    GtkWidget *Romance_logo= gtk_image_new_from_file("romance.jpg");
    gtk_fixed_put(GTK_FIXED(container), Romance_logo, 1000, 500); // Adjust position as needed
    gtk_widget_show(Romance_logo);

   GtkWidget *back1 = gtk_button_new_with_label("<--");
    gtk_fixed_put(GTK_FIXED(container), back1, 30, 30);
    gtk_widget_show(back1);

    GtkWidget *B1= gtk_button_new_with_label("Fiction");
    gtk_fixed_put(GTK_FIXED(container), B1, 200, 860);
    gtk_widget_show(B1);
    g_signal_connect(B1, "clicked", G_CALLBACK(hide_all_widgets), container);

    GtkWidget *B2 = gtk_button_new_with_label("Romance");
    gtk_fixed_put(GTK_FIXED(container), B2, 1250, 820);
    gtk_widget_show(B2);
    g_signal_connect(B2, "clicked", G_CALLBACK(hide_all_widgets), container);

    GtkWidget *B3 = gtk_button_new_with_label("Non Fiction");
    gtk_fixed_put(GTK_FIXED(container), B3, 850, 400);
    gtk_widget_show(B3);

     // Load the image for the "Fashion" category


    g_signal_connect(B3, "clicked", G_CALLBACK(hide_all_widgets), container);
   g_signal_connect(back1, "clicked", G_CALLBACK(return_call), container);

}

void hide_all_widgets(GtkWidget *button, gpointer data) {
    GtkWidget *container = GTK_WIDGET(data);
    gchar *button_label = gtk_button_get_label(GTK_BUTTON(button));
    GList *children = gtk_container_get_children(GTK_CONTAINER(container));
    GList *iter = children;

    if(sub_cat_name){
       previouscat=sub_cat_name;
       sub_cat_name=g_strconcat(button_label,".txt",NULL);

    }
    sub_cat_name=g_strconcat(button_label,".txt",NULL);

    while (iter != NULL) {
        GtkWidget *widget = GTK_WIDGET(iter->data);
        gtk_widget_hide(widget);
        iter = g_list_next(iter);
    }

    g_list_free(children);
    if (g_strcmp0(button_label, "Fashion") == 0 ) {
                   sec_return = gtk_button_get_label(GTK_BUTTON(button));
        Fcategories(container, data);
    } else if (g_strcmp0(button_label, "Electronics") == 0 ) {
       sec_return = gtk_button_get_label(GTK_BUTTON(button));
        Ecategories(container, data);
    }
    else if (g_strcmp0(button_label, "Furniture") == 0 ) {
        sec_return = gtk_button_get_label(GTK_BUTTON(button));
        furniture(container, data);
    }

     else if (g_strcmp0(button_label, "Books") == 0 ) {
         sec_return = gtk_button_get_label(GTK_BUTTON(button));
        books(container, data);
    }
    else if(g_strcmp0(button_label, "  X  ") == 0 ){
            g_print("coin\n");
       actual_main();

    }
    else if (g_strcmp0(button_label, "Add to Books") == 0 ) {
        Add_to_books(container, data);
    }
    else if (g_strcmp0(button_label, "Bedroom") == 0 || g_strcmp0(button_label, "Living Room") == 0 || g_strcmp0(button_label, "Kitchen") == 0|| g_strcmp0(button_label, "Romance") == 0 || g_strcmp0(button_label, "Non Fiction") == 0|| g_strcmp0(button_label, "Fiction") == 0) {
        permanent=g_strconcat(button_label,".txt",NULL);
        sub_category(container, data);
    }
      else if (g_strcmp0(button_label, "Men") == 0  ||g_strcmp0(button_label, "Women") == 0  ||g_strcmp0(button_label, "Kids") == 0 ||g_strcmp0(button_label, "TV") == 0 ||g_strcmp0(button_label, "phones") == 0  || g_strcmp0(button_label, "Laptops") == 0  ) {
        permanent=g_strconcat(button_label,".txt",NULL);
        sub_category(container, data);
    }
    else if(g_strcmp0(button_label, "<<---") == 0){
        if(g_strcmp0(sec_return, "Electronics") == 0)Ecategories(container,data);
        if(g_strcmp0(sec_return, "Fashion") == 0)Fcategories(container,data);
        if(g_strcmp0(sec_return, "Furniture") == 0)furniture(container,data);
         if(g_strcmp0(sec_return, "Books") == 0)books(container,data);
}

    else if(g_strcmp0(button_label, "<<<--") == 0){
             sub_category(container, data);
    }
    else if (g_strcmp0(button_label, "<--") == 0) {
        return_call(container, data);
    }
  else if (g_strcmp0(button_label, "\ne") == 0) {
        profile(container, data);
    }
 else if (g_strcmp0(button_label, "trolly") == 0) {
        clicked_trolly(container, data);
    }
else if (g_strcmp0(button_label, "order") == 0) {
        order(container, data);
    }else if (g_strcmp0(button_label, "seller") == 0){
    seller(container,data);
    }else if(g_strcmp0(button_label, "Search") == 0){
searching(container,data);
}

    else{
       products(previouscat,data);
    }
}
void actual_main() {

    gtk_widget_destroy(signin_success_window);
    gtk_widget_destroy(signin_window);

        GtkWidget *background = gtk_image_new_from_file("wallpaper.jpg");
        gtk_widget_set_size_request(background, 1900, 1000);
        gtk_fixed_put(GTK_FIXED(fixed), background, 0, 0);

        viewproducts("main");

        search_entry = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), search_entry, 1500, 50);
   g_signal_connect(search_entry, "activate", G_CALLBACK(initiate_search), NULL);


  GtkWidget  *search = gtk_button_new_with_label("Search");
    gtk_fixed_put(GTK_FIXED(fixed), search, 1500, 100);
    g_signal_connect(search, "clicked", G_CALLBACK(hide_all_widgets), fixed);


    GtkWidget *logo = gtk_image_new_from_file("logo.png");
    gtk_widget_set_size_request(logo, 1400, 120);
    gtk_fixed_put(GTK_FIXED(fixed), logo, 0, 0);

    GtkWidget *title = gtk_image_new_from_file("title.png");
    gtk_widget_set_size_request(title, 1900, 100);
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    GtkWidget *prof = gtk_button_new_with_label("\ne");
    gtk_fixed_put(GTK_FIXED(fixed), prof, 49, 57);

      GtkWidget *trolly = gtk_button_new_with_label("trolly");
    gtk_fixed_put(GTK_FIXED(fixed), trolly, 100, 300);

    GtkWidget *profile_image = gtk_image_new_from_file("profile.png");
    gtk_fixed_put(GTK_FIXED(fixed), profile_image, 10, 30);


    GtkWidget *Fbutton = gtk_button_new_with_label("Fashion");
    gtk_fixed_put(GTK_FIXED(fixed), Fbutton, 280, 450);
    GtkWidget *fasimg = gtk_image_new_from_file("fashionn.jpg");
    gtk_fixed_put(GTK_FIXED(fixed), fasimg, 100, 200);
    g_signal_connect(Fbutton, "clicked", G_CALLBACK(hide_all_widgets), fixed);

      GtkWidget *order= gtk_button_new_with_label("order");
    gtk_fixed_put(GTK_FIXED(fixed), order, 1700, 100);

    g_print("i\n");

    GtkWidget *Ebutton = gtk_button_new_with_label("Electronics");
    gtk_fixed_put(GTK_FIXED(fixed), Ebutton, 800, 450);
    GtkWidget *fasimg2 = gtk_image_new_from_file("electronics.jpg");
    gtk_fixed_put(GTK_FIXED(fixed), fasimg2, 700, 210);
        g_signal_connect(Ebutton, "clicked", G_CALLBACK(hide_all_widgets), fixed);

    GtkWidget *F1button = gtk_button_new_with_label("Furniture");
    gtk_fixed_put(GTK_FIXED(fixed), F1button, 1140, 450);
    GtkWidget *fasimg1 = gtk_image_new_from_file("furniture.jpg");
    gtk_fixed_put(GTK_FIXED(fixed), fasimg1, 1020, 230);
   GtkWidget *basimg = gtk_image_new_from_file("Books.jpg");
    gtk_fixed_put(GTK_FIXED(fixed), basimg, 1400, 215);

     GtkWidget *Bbutton = gtk_button_new_with_label("Books");
    gtk_fixed_put(GTK_FIXED(fixed), Bbutton, 1500, 450);
return_label = gtk_button_get_label(GTK_BUTTON(Bbutton));

    g_signal_connect(F1button, "clicked", G_CALLBACK(hide_all_widgets), fixed);
    g_signal_connect(Bbutton, "clicked", G_CALLBACK(hide_all_widgets), fixed);
     g_signal_connect(prof, "clicked", G_CALLBACK(hide_all_widgets), fixed);
  g_signal_connect(trolly, "clicked", G_CALLBACK(hide_all_widgets), fixed);
 g_signal_connect(order, "clicked", G_CALLBACK(hide_all_widgets), fixed);

    gtk_widget_show_all(window);

 g_print("i\n");


    return 0;
}

void destroy(GtkWidget *container) {
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(container));

    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        GtkWidget *widget = GTK_WIDGET(iter->data);
        if (widget != container) {
            gtk_widget_destroy(widget);
        }
    }
    g_list_free(children);
}


void OK_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *container = GTK_WIDGET(data);
    destroy(container);
    actual_main();
}

void on_signin_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *username_entry = GTK_WIDGET(data); // Cast the data to GtkWidget pointer
    GtkWidget *password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(data), "password_entry"));

    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

     if (read_user_details("user_details4.workspace", username, password)) {
signin_success_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(signin_success_window), "Sign In Success");
gtk_window_set_default_size(GTK_WINDOW(signin_success_window), 200, 100);
GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
gtk_container_add(GTK_CONTAINER(signin_success_window), vbox);
GtkWidget *label = gtk_label_new("Sign In Successful!");
gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
GtkWidget *button1 = gtk_button_new_with_label("OK");
gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);
    g_signal_connect(button1, "clicked", G_CALLBACK(OK_clicked), fixed);
gtk_widget_show_all(signin_success_window);
set_current_user(username);

    } else {
        // User not found, show error message
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid username or password");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
    }
}

void set_current_user(const gchar *username) {
    if (currentuser != NULL) {
        g_free(currentuser);
    }
    currentuser = g_strdup(username); // Copy the username to currentuser
}

gboolean check_name_exists(const gchar *name) {
    FILE *file = fopen("user_details4.workspace", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            char *stored_name = strtok(line, ",");
            if (strcmp(stored_name, name) == 0) {
                fclose(file);
                return TRUE;
            }
        }
        fclose(file);
    }
    return FALSE;
}

gboolean check_email_exists(const char *email) {
    FILE *file = fopen("user_details4.workspace", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        strtok(line, ","); // Skip name
        strtok(NULL,",");
        char *stored_email = strtok(NULL, ",");
        if (stored_email != NULL && strcmp(stored_email, email) == 0) {
            fclose(file);
            return TRUE;
        }
    }

    fclose(file);
    return FALSE;
}

int check_phone_exists(const char *phone) {
    FILE *file = fopen("user_details4.workspace", "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 0; // Assume phone doesn't exist if file cannot be opened
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        strtok(line, ","); // Skip name

        char *stored_phone = strtok(NULL, ",");
        if (stored_phone != NULL && strcmp(stored_phone, phone) == 0) {
            fclose(file);
            return 1; // Phone exists
        }
    }

    fclose(file);
    return 0; // Phone doesn't exist
}

void on_submit_button_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *signup_window = GTK_WIDGET(data);

    GtkWidget *vbox = GTK_WIDGET(data); // Cast the data to GtkWidget pointer
    GList *children = gtk_container_get_children(GTK_CONTAINER(vbox));

    gchar *name = NULL;
    gchar *phone = NULL;
    gchar *email = NULL;
    gchar *password = NULL;
    gchar *address = NULL;

    while (children != NULL) {
        GtkWidget *child = GTK_WIDGET(children->data);
        if (GTK_IS_ENTRY(child)) {
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(child));
            // Determine which entry field the text belongs to based on the widget's name
            if (strcmp(gtk_widget_get_name(child), "name_entry") == 0) {
                name = g_strdup(text);
            } else if (strcmp(gtk_widget_get_name(child), "phone_entry") == 0) {
                // Check if phone number is exactly 10 digits
                if (strlen(text) != 10) {
                    GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Phone number must be exactly 10 digits");
                    gtk_dialog_run(GTK_DIALOG(error_dialog));
                    gtk_widget_destroy(error_dialog);
                    // Free the allocated memory for user input
                    g_list_free(children);
                    g_free(name);
                    return;
                }
                phone = g_strdup(text);
            } else if (strcmp(gtk_widget_get_name(child), "email_entry") == 0) {
                email = g_strdup(text);
            } else if (strcmp(gtk_widget_get_name(child), "password_entry") == 0) {
                password = g_strdup(text);
            } else if (strcmp(gtk_widget_get_name(child), "address_entry") == 0) {
                address = g_strdup(text);
            }
        }
        children = g_list_next(children);
    }

    // Release the memory allocated for the list of children
    g_list_free(children);

    // Check if the name already exists
    if (check_name_exists(name)) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Name already exists. Please choose a different name.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        // Free the allocated memory for user input
        g_free(name);
        g_free(phone);
        g_free(email);
        g_free(password);
        g_free(address);
        return;
    }
    if (check_email_exists(email)) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Email already exists. Please choose a different Email.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        // Free the allocated memory for user input
        g_free(name);
        g_free(phone);
        g_free(email);
        g_free(password);
        g_free(address);
        return;
    }
    if (check_phone_exists(phone)) {
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Phone number already exists. Please choose a different Phone number.");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        // Free the allocated memory for user input
        g_free(name);
        g_free(phone);
        g_free(email);
        g_free(password);
        g_free(address);
        return;
    }

    // Write the user input to a file
    FILE *file = fopen("user_details4.workspace", "a");
    if (file != NULL) {
        // Write the details to the file
        if (fprintf(file, "%s,%s,%s,%s,%s\n", name, phone, email, password, address) > 0) {
            fclose(file);
        } else {
            fclose(file);
        }
    }
    // Free the allocated memory for user input
    g_free(name);
    g_free(phone);
    g_free(email);
    g_free(password);
    g_free(address);

    // Close the signup window
    gtk_widget_destroy(signup_window);
}
void on_name_entry_activate(GtkWidget *widget, gpointer data) {
    GtkWidget *next_entry = GTK_WIDGET(data);
//next_entry is the widget where it goes after clicking the enter button
    gtk_widget_grab_focus(next_entry);
}

void connect_name_entry_activate(GtkWidget *name_entry, GtkWidget *next_entry) {
    // Connect the "activate" signal of the name entry to the callback function
    g_signal_connect(name_entry, "activate", G_CALLBACK(on_name_entry_activate), next_entry);
}
void on_address_entry_activate(GtkWidget *widget, gpointer data) {
    // Programmatically trigger the "clicked" signal of the sign-in button when Enter key is pressed in the password entry
    GtkWidget *submit_button = GTK_WIDGET(data);
    gtk_button_clicked(GTK_BUTTON(submit_button));
}

void create_and_display_signup_window() {
   GtkWidget *signup_window;
    GtkWidget *name_label, *phone_label, *email_label, *password_label, *address_label;
    GtkWidget *name_entry, *phone_entry, *email_entry, *password_entry, *address_entry;
    GtkWidget *submit_button;
    GtkWidget *vbox;

    // Create a new window
    signup_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(signup_window), "Sign Up");
    gtk_container_set_border_width(GTK_CONTAINER(signup_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(signup_window), 300, 300);

    // Create a vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(signup_window), vbox);

    // Name
    name_label = gtk_label_new("Name:");
    gtk_box_pack_start(GTK_BOX(vbox), name_label, FALSE, FALSE, 0);
    name_entry = gtk_entry_new();
    gtk_widget_set_name(name_entry, "name_entry"); // Set name for identification
    gtk_box_pack_start(GTK_BOX(vbox), name_entry, FALSE, FALSE, 0);

    // Phone
    phone_label = gtk_label_new("Phone:");
    gtk_box_pack_start(GTK_BOX(vbox), phone_label, FALSE, FALSE, 0);
    phone_entry = gtk_entry_new();
    gtk_widget_set_name(phone_entry, "phone_entry");
    gtk_entry_set_max_length(GTK_ENTRY(phone_entry), 10); // Limit phone number length
    gtk_entry_set_input_purpose(GTK_ENTRY(phone_entry), GTK_INPUT_PURPOSE_PHONE); // Set input purpose to phone number
    gtk_box_pack_start(GTK_BOX(vbox), phone_entry, FALSE, FALSE, 0);

    // Email
    email_label = gtk_label_new("Email:");
    gtk_box_pack_start(GTK_BOX(vbox), email_label, FALSE, FALSE, 0);
    email_entry = gtk_entry_new();
    gtk_widget_set_name(email_entry, "email_entry");
    gtk_box_pack_start(GTK_BOX(vbox), email_entry, FALSE, FALSE, 0);

    // Password
    password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 0);
    password_entry = gtk_entry_new();
    gtk_widget_set_name(password_entry, "password_entry");
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password characters
    gtk_entry_set_input_purpose(GTK_ENTRY(password_entry), GTK_INPUT_PURPOSE_PASSWORD); // Set input purpose to password
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);

    // Address
    address_label = gtk_label_new("Address:");
    gtk_box_pack_start(GTK_BOX(vbox), address_label, FALSE, FALSE, 0);
    address_entry = gtk_entry_new();
    gtk_widget_set_name(address_entry, "address_entry");
    gtk_box_pack_start(GTK_BOX(vbox), address_entry, FALSE, FALSE, 0);

    // Add a separator
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Submit button
    submit_button = gtk_button_new_with_label("Submit");
    gtk_box_pack_start(GTK_BOX(vbox), submit_button, FALSE, FALSE, 0);

    // Connect the "Submit" button click event to the callback function
    g_signal_connect(submit_button, "clicked", G_CALLBACK(on_submit_button_clicked), vbox);

    connect_name_entry_activate(name_entry, phone_entry);
    connect_name_entry_activate(phone_entry, email_entry);
    connect_name_entry_activate(email_entry, password_entry);
    connect_name_entry_activate(password_entry, address_entry);
    g_signal_connect(password_entry, "activate", G_CALLBACK(on_address_entry_activate), submit_button);

    // Show all widgets
    gtk_widget_show_all(signup_window);

}

void create_and_display_signin_window() {

    GtkWidget *username_label, *password_label;
    GtkWidget *username_entry, *password_entry;
    GtkWidget *signin_button;
    GtkWidget *vbox;

    // Create a new window
    signin_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(signin_window), "Sign In");
    gtk_container_set_border_width(GTK_CONTAINER(signin_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(signin_window), 300, 200);

    // Create a vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(signin_window), vbox);

    // Username
    username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(vbox), username_label, FALSE, FALSE, 0);
    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);

    // Password
    password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 0);
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password characters
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);

    // Add a separator
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Sign In button
    signin_button = gtk_button_new_with_label("Sign In");
    gtk_box_pack_start(GTK_BOX(vbox), signin_button, FALSE, FALSE, 0);

    // Connect the "Sign In" button click event to the callback function
    g_signal_connect(signin_button, "clicked", G_CALLBACK(on_signin_button_clicked), username_entry);


 connect_name_entry_activate(username_entry, password_entry);

    // Connect the "activate" signal of the password entry to click the sign-in button on Enter key press
  g_signal_connect(password_entry, "activate", G_CALLBACK(on_address_entry_activate), signin_button);

    // Store the password entry widget as user data for later retrieval
    g_object_set_data(G_OBJECT(username_entry), "password_entry", password_entry);

    // Show all widgets
    gtk_widget_show_all(signin_window);
}



void save_details(GtkWidget *widget, gpointer grid) {
    // Get the entry widgets from the grid
    GtkWidget *shop_name_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
    GtkWidget *address_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 1);
    GtkWidget *password_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 2);
 GtkWidget *phone_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 3);
 GtkWidget *past_entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, 4);
    // Get the text from the entry widgets
    const char *shop_name = gtk_entry_get_text(GTK_ENTRY(shop_name_entry));
    const char *address = gtk_entry_get_text(GTK_ENTRY(address_entry));
    const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
 const char *phone = gtk_entry_get_text(GTK_ENTRY(phone_entry));
const char *past= gtk_entry_get_text(GTK_ENTRY(past_entry));
    // Open the file in append mode
    FILE *file = fopen("seller.txt", "a");
    if (file != NULL) {
        // Write the details to the file
        fprintf(file, "%s,%s,%s,%d,%s\n", shop_name, address, password,phone,past);

        // Close the file
        fclose(file);

        // Display a message dialog to inform the user
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Details saved successfully!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        // Display an error message if file cannot be opened
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error: Failed to save details!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}
gchar  *cate;
    int row = 0;

void save_entries() {
    FILE *file = fopen(cate, "w");
    if (file == NULL) {
        g_print("Error opening file.\n");
        return;
    }


        for (int r = 0;r<row; r++) {
        for (int col = 0; col < 13; col++) {


            GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), 1, r);
        if(entry==NULL){

                                entry = gtk_grid_get_child_at(GTK_GRID(grid), col, r);
                            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
                                        if(r) fprintf(file, "]\n", text);

                     fprintf(file, "[%s\n", text);
        break;

        }
        if(col==1){
                    GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), col, r);
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
            fprintf(file, "%s\n", text);
            continue;
        }
        if(col==12){
                    GtkWidget *entry = gtk_grid_get_child_at(GTK_GRID(grid), col, r);
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
            fprintf(file, "%s\n;", text);
            continue;
        }
          entry = gtk_grid_get_child_at(GTK_GRID(grid), col, r);
            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
            fprintf(file, "%s|", text);


        }
        fprintf(file, "\n");
    }

        fprintf(file, "]");

    fclose(file);
    g_print("Entries saved successfully.\n");
}


void add_row() {
GtkWidget *entryy = gtk_grid_get_child_at(GTK_GRID(grid), 1,3 );

                            const gchar *text = gtk_entry_get_text(GTK_ENTRY(entryy));
                     printf( "[%s\n\n\n\n\n\n\n", text);


                     if(entryy==NULL){
                        printf("hhh\n");
                     }
    row=row+1;
    for (int col = 0; col < 13; col++) {
        GtkWidget *entry = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry, col, row, 1, 1);
        gtk_widget_show(entry);
    }
}


void crad_prod(GtkWidget *button, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);

    GtkWidget *add_button = gtk_button_new_with_label("Add Row");
         gtk_fixed_put(GTK_FIXED(fixed), add_button, 200, 90);
    gtk_widget_show(add_button);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_row), NULL);

    GtkWidget *save_button = gtk_button_new_with_label("Save Entries");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_entries), NULL);
    gtk_fixed_put(GTK_FIXED(fixed), save_button, 200, 120);
    gtk_widget_show(save_button);


    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_fixed_put(GTK_FIXED(window), scrolled_window, 100, 150);
    gchar *b_label = gtk_button_get_label(GTK_BUTTON(button));

      cate=g_strconcat(b_label,".txt",NULL);

      FILE *file = fopen(cate, "r");
    if (file == NULL) {
        return 1;
    }
                    char line[1024];

   Category *head = NULL, *currentCategory = NULL;
    char currentPP[100] = {0};
    char currentProductName[100] = {0};
    int currentMRP = 0;
    int currentPrice = 0;
    float currentRating = 0.0f;
    char currentShortDes[100] = {0};
    int isReadingProduct = 0;
    char currdetails[500]={0};
    char img1[20]={0};
    char img2[20]={0};
    char img3[20]={0};
    int offer1=0;
    char offer2[50]={0};
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Remove newline character

        if (line[0] == '[') { // Start new category
            sscanf(line, "[%[^]]", line);
            currentCategory = createCategory(line);
            if (!head) {
                head = currentCategory;
            } else {
                Category *temp = head;
                while (temp->next) {
                    temp = temp->next;
                }
                temp->next = currentCategory;
            }
        } else if (line[0] == ']') { // End current category
            isReadingProduct = 0;
        } else if (line[0] == ';') { // End current product
            Product *newProduct = createProduct(currentPP, img1, img2, img3, currentProductName, currentMRP, currentPrice, currentRating, currentShortDes, currdetails, offer1, offer2);

            addProductToCategory(currentCategory, newProduct);
            isReadingProduct = 0; // Reset product reading state
        } else if (isReadingProduct) {
            // Parse additional product details if needed
            if (strstr(line, "|")) {  // Check if line contains product details
                sscanf(line, "%[^|]|%[^|]|%[^|]|%[^|]|%d|%d|%f|%[^|]|%[^|]|%d|%[^|]",img1,img2,img3,currentProductName, &currentMRP, &currentPrice, &currentRating, currentShortDes,currdetails,&offer1,offer2);
            }
        } else {
            // It's the product PP (Product Code)
            strcpy(currentPP, line);
            isReadingProduct = 1; // Next lines are part of this product until ';' encountered
        }
    }
    reqcategroy=head;

    fclose(file);
//g_print(head->name);



    grid = gtk_grid_new(); // Create a new grid for each category

    Product *currentProduct = head->products;

    while (currentProduct) {
printf("%s",currentProduct->offer2);

if (currentProduct->next != NULL) {
    } else if(currentProduct->next == NULL ) {
        if (head->next == NULL) {
            break;
        }if(row){
        head = head->next;
                currentProduct = head->products;

        }
        printf("%d",row);
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), head->name);
        gtk_grid_attach(GTK_GRID(grid), entry, 0, row, 1, 1);
                row = row + 1;
                // Move to the first product in the next category
    }

    if(row==0){
entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), head->name);
        gtk_grid_attach(GTK_GRID(grid), entry, 0, row, 1, 1);
                row = row + 1;
    }

        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->PP);
        gtk_grid_attach(GTK_GRID(grid), entry, 0, row, 1, 1);

        // Entry for Image 1
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->img1);
        gtk_grid_attach(GTK_GRID(grid), entry, 1, row, 1, 1);

        // Entry for Image 2
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->img2);
        gtk_grid_attach(GTK_GRID(grid), entry, 2, row, 1, 1);

        // Entry for Image 3
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->img3);
        gtk_grid_attach(GTK_GRID(grid), entry, 3, row, 1, 1);
printf("%s\n",currentProduct->offer2);
        // Entry for Product Name
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->name);
        gtk_grid_attach(GTK_GRID(grid), entry, 4, row, 1, 1);

        // Entry for MRP
        entry = gtk_entry_new();
        gchar *mrp_text = g_strdup_printf("%d", currentProduct->MRP);
        gtk_entry_set_text(GTK_ENTRY(entry), mrp_text);
        g_free(mrp_text);
        gtk_grid_attach(GTK_GRID(grid), entry, 5, row, 1, 1);

        // Entry for Price
        entry = gtk_entry_new();
        gchar *price_text = g_strdup_printf("%d", currentProduct->price);
        gtk_entry_set_text(GTK_ENTRY(entry), price_text);
        g_free(price_text);
        gtk_grid_attach(GTK_GRID(grid), entry, 6, row, 1, 1);

        // Entry for Rating
        entry = gtk_entry_new();
        gchar *rating_text = g_strdup_printf("%.1f", currentProduct->rating);
        gtk_entry_set_text(GTK_ENTRY(entry), rating_text);
        g_free(rating_text);
        gtk_grid_attach(GTK_GRID(grid), entry, 7, row, 1, 1);

        // Entry for Short Description
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->short_des);
        gtk_grid_attach(GTK_GRID(grid), entry, 8, row, 1, 1);

        // Entry for Additional Details
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->details);
        gtk_grid_attach(GTK_GRID(grid), entry, 9, row, 1, 1);

        // Entry for Offer 1
        entry = gtk_entry_new();
        gchar *offer1_text = g_strdup_printf("%d", currentProduct->discount);
        gtk_entry_set_text(GTK_ENTRY(entry), offer1_text);
        g_free(offer1_text);
        gtk_grid_attach(GTK_GRID(grid), entry, 10, row, 1, 1);

        // Entry for Offer 2
        entry = gtk_entry_new();
        gtk_entry_set_text(GTK_ENTRY(entry), currentProduct->offer2);
        gtk_grid_attach(GTK_GRID(grid), entry, 11, row, 1, 1);

        currentProduct = currentProduct->next;

g_print("hh\n");

  row = row + 1;

    }

    gtk_container_add(GTK_CONTAINER(scrolled_window), grid); // Add the grid to scrolled window




    // Change the size of the scrolled window
    gtk_widget_set_size_request(scrolled_window, 1700, 3000);

    // Show all widgets inside the scrolled window
    gtk_widget_show_all(scrolled_window);
}

void Add_to_books(GtkWidget *container, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data); // Cast data back to GtkWidget pointer
    GtkWidget *B1 = gtk_button_new_with_label("Fiction");
    gtk_fixed_put(GTK_FIXED(container), B1, 200, 50);
    gtk_widget_show(B1);
    g_signal_connect(B1, "clicked", G_CALLBACK(crad_prod), data); // Pass window as data
}

void actual_main1() {




gtk_widget_destroy(signin_success_window);
    gtk_widget_destroy(seller_signin_window);

    GtkWidget *fashionbutton=gtk_button_new_with_label("Add to Books");
    gtk_fixed_put(GTK_FIXED(fixed), fashionbutton, 299, 507);
    g_signal_connect(fashionbutton, "clicked", G_CALLBACK(hide_all_widgets), fixed);



    gtk_widget_show_all(fixed);
}

void OK_clickeds(GtkWidget *widget, gpointer data) {

    GtkWidget *container = GTK_WIDGET(data);
    destroy(container);
    actual_main1();
}
void create_and_display_signup_windows() {
    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Sign up");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // Create a new grid with 2 columns and 6 rows
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Text labels for the left side
    const char *labels[] = {"Shop Name:", "Address:", "Password:", "Phone No.","Past Expereince"};

    // Add labels and entry widgets to the grid
    for (int i = 0; i < 5; i++) {
        // Label on the left
        GtkWidget *label = gtk_label_new(labels[i]);
        gtk_widget_set_halign(label, GTK_ALIGN_END);
        gtk_grid_attach(GTK_GRID(grid), label, 0, i, 1, 1);

        // Entry widget on the right
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "");
        gtk_grid_attach(GTK_GRID(grid), entry, 1, i, 1, 1);
    }

    // Button to save details
    GtkWidget *save_button = gtk_button_new_with_label("Save");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_details), grid);
    gtk_grid_attach(GTK_GRID(grid), save_button, 1, 5, 1, 1);

    // Show all widgets
    gtk_widget_show_all(window);
}

// Callback function for the "Sign In" button click event
void on_signin_button_clickeds(GtkWidget *widget, gpointer data) {
    GtkWidget *username_entry = GTK_WIDGET(data);
    const gchar *username = gtk_entry_get_text(GTK_ENTRY(username_entry));

    // Retrieve the password entry widget from user data
    GtkWidget *password_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(username_entry), "password_entry"));
    const gchar *password = gtk_entry_get_text(GTK_ENTRY(password_entry));

    // Check username and password against data in the file
    FILE *file = fopen("seller.txt", "r");
    if (file != NULL) {
        char buffer[256];
        int found = 0;
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Parse the line from the file
            char *saved_username = strtok(buffer, ",");
            char *saved_password = strtok(NULL, ",");
            saved_password[strcspn(saved_password, "\n")] = 0; // Remove newline character

            // Compare username and password
            if (strcmp(username, saved_username) == 0 && strcmp(password, saved_password) == 0) {
                found = 1;
                break;
            }
        }
        fclose(file);

        // Check if the credentials were found
        if (found) {
            // Successful sign-in
signin_success_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(signin_success_window), "Sign In Success");
gtk_window_set_default_size(GTK_WINDOW(signin_success_window), 200, 100);
GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
gtk_container_add(GTK_CONTAINER(signin_success_window), vbox);
GtkWidget *label = gtk_label_new("Sign In Successful!");
gtk_box_pack_start(GTK_BOX(vbox), label, TRUE, TRUE, 0);
GtkWidget *button1 = gtk_button_new_with_label("OK");
gtk_box_pack_start(GTK_BOX(vbox), button1, TRUE, TRUE, 0);
    g_signal_connect(button1, "clicked", G_CALLBACK(OK_clickeds), fixed);
gtk_widget_show_all(signin_success_window);
set_current_user(username);

        } else {
           // User not found, show error message
        GtkWidget *error_dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Invalid username or password");
        gtk_dialog_run(GTK_DIALOG(error_dialog));
        gtk_widget_destroy(error_dialog);
        }
    } else {
        // Display error message if file cannot be opened
        g_print("Error: Failed to open file for reading.\n");
    }
}
void create_and_display_signin_windows() {
    GtkWidget *username_label, *password_label;
    GtkWidget *username_entry, *password_entry;
    GtkWidget *signin_button;
    GtkWidget *vbox;

    // Create a new window
    seller_signin_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(seller_signin_window), "Sign In");
    gtk_container_set_border_width(GTK_CONTAINER(seller_signin_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(seller_signin_window), 300, 200);

    // Create a vertical box container
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(seller_signin_window), vbox);

    // Username
    username_label = gtk_label_new("Username:");
    gtk_box_pack_start(GTK_BOX(vbox), username_label, FALSE, FALSE, 0);
    username_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);

    // Password
    password_label = gtk_label_new("Password:");
    gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 0);
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE); // Hide password characters
    gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);

    // Add a separator
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(vbox), separator, FALSE, FALSE, 5);

    // Sign In button
    signin_button = gtk_button_new_with_label("SIGN IN");
    gtk_box_pack_start(GTK_BOX(vbox), signin_button, FALSE, FALSE, 0);

    g_signal_connect(signin_button, "clicked", G_CALLBACK(on_signin_button_clickeds), username_entry);

connect_name_entry_activate(username_entry, password_entry);

 g_signal_connect(password_entry, "activate", G_CALLBACK(on_address_entry_activate), signin_button);


    // Store the password entry widget as user data for later retrieval
    g_object_set_data(G_OBJECT(username_entry), "password_entry", password_entry);

    // Show all widgets
    gtk_widget_show_all(seller_signin_window);
}



int secret_code;
int numofAttempts = 1;
int flag2 = 0;
int winner;

int random_number() {
    int x;
    do {
        x = rand() % 9000 + 1000;
    } while (!has_unique_digits(x));
    return x;
}

int has_unique_digits(int num) {
    int digits[10] = {0};
    while (num > 0) {
        int digit = num % 10;
        if (digits[digit] == 1) {
            return 0;
        }
        digits[digit] = 1;
        num /= 10;
    }
    return 1;
}

void on_guess_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *entry = GTK_WIDGET(data);
    const gchar *guess_text = gtk_entry_get_text(GTK_ENTRY(entry));
    int player_guess = atoi(guess_text);

    if (!flag2) {
        secret_code = random_number();
        flag2 = 1; // Game started
    }

    GtkWidget *dialog;
    if (checking(player_guess, secret_code)) {
        winner = 1;
        flag2 = 2; // Game over
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Congratulations! You guessed the secret code: %d", secret_code);
    } else {
        int bulls, cows;
        counting_bulls_and_cows(player_guess, secret_code, &bulls, &cows);
        dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Bulls: %d, Cows: %d", bulls, cows);
    }

    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int checking(int guess, int secret_code) {
    return guess == secret_code;
}

void counting_bulls_and_cows(int guess, int secret_code, int *bulls, int *cows) {
    int code_digits[4];
    int guess_digits[4];
    *bulls = 0;
    *cows = 0;

    for (int i = 0; i < 4; i++) {
        code_digits[i] = secret_code % 10;
        guess_digits[i] = guess % 10;
        secret_code /= 10;
        guess /= 10;
    }

    for (int i = 0; i < 4; i++) {
        if (guess_digits[i] == code_digits[i]) {
            (*cows)++;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (guess_digits[i] == code_digits[j] && i != j) {
                (*bulls)++;
            }
        }
    }
}void close_the_entertainment_window(GtkWidget *widget, gpointer data) {
    gtk_widget_destroy(GTK_WIDGET(data));
}

void show_game_window() {
    srand(time(NULL));
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Bulls and Cows Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *instructions_label = gtk_label_new(NULL);
    gtk_box_pack_start(GTK_BOX(vbox), instructions_label, FALSE, FALSE, 0);

    gtk_label_set_markup(GTK_LABEL(instructions_label), "<span size='large'><b>Hello There! Welcome to Mind Match: The Guessing Game of Bulls and Cows.\nThe rules of the game are:\n1. The code is a simple 4-digit number. All the digits are unique, i.e., none of them are repeated.\n2. For every guess you make, number of bulls and cows are shown.\n   -Bulls show the number of digits that match the secret code with your guess, but are in the wrong position.\n   -Cows show the number of digits that match the secret code with your guess and are in the right position.\nClick on 'Guess' to start the game!\n</b></span>");
    gtk_widget_override_font(instructions_label, pango_font_description_from_string("Menlo 13"));

    GtkWidget *guess_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), guess_entry, FALSE, FALSE, 0);

    GtkWidget *guess_button = gtk_button_new_with_label("Guess");
    gtk_widget_set_size_request(guess_button, 250, 50);
    g_signal_connect(guess_button, "clicked", G_CALLBACK(on_guess_button_clicked), guess_entry);
    gtk_box_pack_start(GTK_BOX(vbox), guess_button, FALSE, FALSE, 0);

    GtkWidget *close_button = gtk_button_new_with_label("Close");
    gtk_widget_set_size_request(close_button, 250, 50);
    g_signal_connect(close_button, "clicked", G_CALLBACK(close_the_entertainment_window), window);
    gtk_box_pack_start(GTK_BOX(vbox), close_button, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
}
typedef struct {
    GtkWidget *button;
    GtkWidget *label;
    int score;
    int time_remaining;
    int total_score;
} GameData;

// GTK callback function for "Click Me" button
void on_click_button_clicked(GtkWidget *widget, gpointer data) {
    GameData *gameData = (GameData *)data;
    gameData->score++;
    char score_text[50];
    sprintf(score_text, "Score: %d", gameData->score);
    gtk_label_set_text(GTK_LABEL(gameData->label), score_text);
}

// Function to update the timer label
gboolean update_timer_label(gpointer data) {
    GameData *gameData = (GameData *)data;
    gameData->time_remaining--;
    char time_text[50];
    sprintf(time_text, "Time Remaining: %d seconds", gameData->time_remaining);
    gtk_label_set_text(GTK_LABEL(gameData->label), time_text);

    if (gameData->time_remaining == 0) {
        gtk_button_set_label(GTK_BUTTON(gameData->button), "Game Over");
        gtk_widget_set_sensitive(gameData->button, FALSE);
        gameData->total_score = gameData->score; // Store total score
        char score_text[50];
        sprintf(score_text, "Total Score: %d", gameData->total_score);
        gtk_label_set_text(GTK_LABEL(gameData->label), score_text);
        return FALSE; // Stop the timer
    }

    return TRUE; // Continue the timer
}

void play_clicking_game() {
    // Initialize GTK
    gtk_init(NULL, NULL);

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Clicking Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a box to hold widgets vertically
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create a button for clicking
    GtkWidget *click_button = gtk_button_new_with_label("Click Me");
    gtk_box_pack_start(GTK_BOX(vbox), click_button, TRUE, TRUE, 0);

    // Create a label to display score and time remaining
    GtkWidget *score_label = gtk_label_new("Score: 0");
    gtk_box_pack_start(GTK_BOX(vbox), score_label, TRUE, TRUE, 0);

    // Initialize game data
    GameData gameData;
    gameData.button = click_button;
    gameData.label = score_label;
    gameData.score = 0;
    gameData.time_remaining = 10; // Set the game time limit to 10 seconds
    gameData.total_score = 0;

    // Connect the button click signal to the callback function
    g_signal_connect(click_button, "clicked", G_CALLBACK(on_click_button_clicked), &gameData);

    // Start the timer to update the time remaining label every second
    g_timeout_add_seconds(1, update_timer_label, &gameData);

    // Show all widgets
    gtk_widget_show_all(window);

    // Run the GTK main loop
    gtk_main();
}

enum {
    EMPTY,
    PLAYER_X,
    PLAYER_O
};

int player = PLAYER_X;
int board[3][3] = {{EMPTY}};

GtkWidget *buttons[3][3];

void update_board(int row, int col) {
    if (board[row][col] == EMPTY) {
        board[row][col] = player;
        if (player == PLAYER_X) {
            gtk_button_set_label(GTK_BUTTON(buttons[row][col]), "X");
            player = PLAYER_O;
        } else {
            gtk_button_set_label(GTK_BUTTON(buttons[row][col]), "O");
            player = PLAYER_X;
        }
        int winner = check_winner();
        if (winner != EMPTY) {
            char *message;
            if (winner == PLAYER_X) {
                message = "Player X wins!";
            } else if (winner == PLAYER_O) {
                message = "Player O wins!";
            } else {
                message = "It's a draw!";
            }
            GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, message);
            gtk_dialog_run(GTK_DIALOG(dialog));
            gtk_widget_destroy(dialog);
            reset_game();
        }
    }
}

int check_winner() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY) {
            return board[i][0];
        }
    }
    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != EMPTY) {
            return board[0][j];
        }
    }
    // Check diagonals
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY) ||
        (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)) {
        return board[1][1];
    }
    // Check for draw
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                return EMPTY; // Game not finished yet
            }
        }
    }
    return 0; // Draw
}

void reset_game() {
    // Clear the board
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = EMPTY;
            gtk_button_set_label(GTK_BUTTON(buttons[i][j]), "");
        }
    }
    player = PLAYER_X;
}

void button_clicked(GtkWidget *widget, gpointer data) {
    int *position = (int *)data;
    int row = position[0];
    int col = position[1];
    update_board(row, col);
}

GtkWidget *create_button(int row, int col) {
    GtkWidget *button = gtk_button_new_with_label("");
    gtk_widget_set_size_request(button, 100, 100);
    int *position = malloc(sizeof(int) * 2);
    position[0] = row;
    position[1] = col;
    g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), position);
    return button;
}

GtkWidget *create_window() {
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    return window;
}

GtkWidget *create_grid() {
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    return grid;
}

void create_board(GtkWidget *grid) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            buttons[i][j] = create_button(i, j);
            gtk_grid_attach(GTK_GRID(grid), buttons[i][j], j, i, 1, 1);
        }
    }
}

void play_tic_tac_toe_game() {
    GtkWidget *window = create_window();
    GtkWidget *grid = create_grid();
    create_board(grid);
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_widget_show_all(window);
    gtk_main();
}

void games(GtkWidget *container, gpointer data) {

    GtkWidget *G1 = gtk_button_new_with_label("Mind Match");
    gtk_fixed_put(GTK_FIXED(container), G1, 200, 860);
    gtk_widget_show(G1);
    g_signal_connect(G1, "clicked", G_CALLBACK(show_game_window), NULL);

    GtkWidget *G2 = gtk_button_new_with_label("Press the Button");
    gtk_fixed_put(GTK_FIXED(container), G2, 1250, 820);
    gtk_widget_show(G2);
    g_signal_connect(G2, "clicked", G_CALLBACK(  play_clicking_game), NULL);

    GtkWidget *G3 = gtk_button_new_with_label("Tic-Tac-Toe");
    gtk_fixed_put(GTK_FIXED(container), G3, 850, 400);
    gtk_widget_show(G3);
    g_signal_connect(G3, "clicked", G_CALLBACK(  play_tic_tac_toe_game), NULL);
}

void login_page_clicked(GtkWidget *widget, gpointer data) {
    // Close the entertainment window
    gtk_widget_destroy(GTK_WIDGET(data));

    // Show the main window again
    gtk_widget_show_all(window);
}

void entertainment_clicked() {
    // Create a new window
    GtkWidget *entertainment_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(entertainment_window), "Games");
    gtk_container_set_border_width(GTK_CONTAINER(entertainment_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(entertainment_window), 2000, 1000);

    // Create a container to hold the widgets
    GtkWidget *container = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(entertainment_window), container);

    // Call the games function to add buttons to the container
    games(container, NULL);

    // Create a button to go back to the main window
    GtkWidget *login_button = gtk_button_new_with_label("Login Page");
    gtk_fixed_put(GTK_FIXED(container), login_button, 30, 20);
    gtk_widget_set_size_request(login_button, 200, 50);
    g_signal_connect(login_button, "clicked", G_CALLBACK(login_page_clicked), entertainment_window);

    // Show all widgets and run the new window's main loop
    gtk_widget_show_all(entertainment_window);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Tiny Trolly");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 1850, 1000);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    fixed = gtk_fixed_new();

    GtkWidget *entry = gtk_entry_new();
    gtk_fixed_put(GTK_FIXED(fixed), entry, 1500, 50);

    GtkWidget *background = gtk_image_new_from_file("wallpaper.jpg");
    gtk_widget_set_size_request(background, 1900, 1000);
    gtk_fixed_put(GTK_FIXED(fixed), background, 0, 0);

    GtkWidget *logo = gtk_image_new_from_file("logo.png");
    gtk_widget_set_size_request(logo, 1400, 120);
    gtk_fixed_put(GTK_FIXED(fixed), logo, 0, 0);

    GtkWidget *title = gtk_image_new_from_file("title.png");
    gtk_widget_set_size_request(title, 1900, 100);
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    label3 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label3), "<span size='large'><b>CUSTOMER</b></span>");
    gtk_widget_override_font(label3, pango_font_description_from_string("Arial Bold 16"));
    gtk_fixed_put(GTK_FIXED(fixed), label3, 1310, 280);

    label4= gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label4), "<span size='large'><b>SELLER</b></span>");
    gtk_widget_override_font(label4, pango_font_description_from_string("Arial Bold 16"));
    gtk_fixed_put(GTK_FIXED(fixed), label4, 400, 280);

    button_signin = gtk_button_new_with_label("Sign In");
    gtk_fixed_put(GTK_FIXED(fixed), button_signin, 1300, 500);
    gtk_widget_set_size_request(button_signin, 250, 50);
    g_signal_connect(button_signin, "clicked", G_CALLBACK(create_and_display_signin_window), NULL);

    button_signin1 = gtk_button_new_with_label("SIGN IN");
    gtk_fixed_put(GTK_FIXED(fixed), button_signin1, 350, 500);
    gtk_widget_set_size_request(button_signin1, 250, 50);
    g_signal_connect(button_signin1, "clicked", G_CALLBACK(create_and_display_signin_windows), NULL);

    button_entertain = gtk_button_new_with_label("Entertainment");
    gtk_fixed_put(GTK_FIXED(fixed), button_entertain, 800, 800);
    gtk_widget_set_size_request(button_entertain, 250, 50);
    g_signal_connect(button_entertain, "clicked", G_CALLBACK(entertainment_clicked), NULL);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span size='larger'>New to Tiny Trolly?</span>");
    gtk_fixed_put(GTK_FIXED(fixed), label, 1330, 360);

    label2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label2), "<span size='larger'>Already a member?</span>");
    gtk_fixed_put(GTK_FIXED(fixed), label2, 1330, 460);

    label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label), "<span size='larger'>New to Tiny Trolly?</span>");
    gtk_fixed_put(GTK_FIXED(fixed), label, 380, 360);

    label2 = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(label2), "<span size='larger'>Already a member?</span>");
    gtk_fixed_put(GTK_FIXED(fixed), label2, 380, 460);

    button_signup1 = gtk_button_new_with_label("SIGN UP");
    gtk_fixed_put(GTK_FIXED(fixed), button_signup1, 350, 400);
    gtk_widget_set_size_request(button_signup1, 250, 50);
    g_signal_connect(button_signup1, "clicked", G_CALLBACK(create_and_display_signup_windows), NULL);

    button_signup = gtk_button_new_with_label("Sign Up");
    gtk_fixed_put(GTK_FIXED(fixed), button_signup, 1300, 400);
    gtk_widget_set_size_request(button_signup, 250, 50);
    g_signal_connect(button_signup, "clicked", G_CALLBACK(create_and_display_signup_window), NULL);

    gtk_container_add(GTK_CONTAINER(scrolled_window), fixed);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    gtk_widget_show_all(window);
    gtk_main();
}

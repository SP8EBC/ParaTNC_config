#pragma once
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>

// Can be changed without further modifications
#define TABLE_MAX_COLS 14

typedef enum
{
    BORDER_NONE,
    BORDER_SINGLE,
    BORDER_DOUBLE
} TableBorderStyle;

typedef enum
{
    H_ALIGN_LEFT,
    H_ALIGN_RIGHT,
    H_ALIGN_CENTER // Rounded to the left
} TableHAlign;

typedef enum
{
    V_ALIGN_TOP,
    V_ALIGN_BOTTOM,
    V_ALIGN_CENTER // Rounded to the top
} TableVAlign;

struct Cell
{
    char *text;         // Actual content to be displayed
    size_t text_height; // Number of lines
    size_t text_width;  // Maximum width of lines

    // Settings
    TableHAlign h_align;           // Non default, how to place text in col width
    TableVAlign v_align;           // Non default, how to place text in col width
    TableBorderStyle border_left;  // Non-default border left
    TableBorderStyle border_above; // Non-default border above
    size_t span_x;                 // How many cols to span over
    size_t span_y;                 // How many rows to span over

    // Generated
    bool override_v_align;      // Default set for each col in table
    bool override_h_align;      // Default set for each col in table
    bool override_border_left;  // Default set for each col in table
    bool override_border_above; // Default set in row

    bool is_set;          // Indicates whether data is valid
    bool text_needs_free; // When set to true, text will be freed on free_table
    size_t x;             // Column position
    size_t y;             // Row position
    struct Cell *parent;  // Cell that spans into this cell
};

struct Row
{
    struct Cell cells[TABLE_MAX_COLS]; // All cells of this row from left to right
    struct Row *next_row;              // Pointer to next row or NULL if last row
    TableBorderStyle border_above;     // Default border above (can be overwritten in cell)
    int border_above_counter;       // Counts cells that override their border_above
};

struct Table
{
    size_t num_cols;                         // Number of columns (max. of num_cells over all rows)
    size_t num_rows;                         // Number of rows (length of linked list)
    struct Row *first_row;                   // Start of linked list to rows
    struct Row *curr_row;                    // Marker of row of next inserted cell
    size_t curr_col;                         // Marker of col of next inserted cell
    TableBorderStyle borders_left[TABLE_MAX_COLS]; // Default left border of cols
    TableHAlign h_aligns[TABLE_MAX_COLS];          // Default horizontal alignment of cols
    TableVAlign v_aligns[TABLE_MAX_COLS];          // Default vertical alignment of cols
    int border_left_counters[TABLE_MAX_COLS];   // Counts cells that override their border_left
};

typedef struct Table Table;


#ifdef __cplusplus
extern "C" {
#endif

// Data and printing
Table *get_empty_table();
void print_table(Table *table);
void fprint_table(Table *table, FILE *stream);
void free_table(Table *table);

// Control
void set_position(Table *table, size_t x, size_t y);
void next_row(Table *table);

// Cell insertion
void add_empty_cell(Table *table);
void add_cell(Table *table, const char *text);
void add_cells(Table *table, size_t num_cells, ...);
void add_cell_gc(Table *table, char *text);
void add_cell_fmt(Table *table, const char *fmt, ...);
void add_cell_vfmt(Table *table, const char *fmt, va_list args);
void add_cells_from_array(Table *table, size_t width, size_t height, const char **array);

// Settings
void set_default_alignments(Table *table, size_t num_alignments, const TableHAlign *hor_aligns, const TableVAlign *vert_aligns);
void override_vertical_alignment(Table *table, TableVAlign align);
void override_horizontal_alignment(Table *table, TableHAlign align);
void override_vertical_alignment_of_row(Table *table, TableVAlign align);
void override_horizontal_alignment_of_row(Table *table, TableHAlign align);
void set_hline(Table *table, TableBorderStyle style);
void set_vline(Table *table, size_t index, TableBorderStyle style);
void make_boxed(Table *table, TableBorderStyle style);
void set_all_vlines(Table *table, TableBorderStyle style);
void override_left_border(Table *table, TableBorderStyle style);
void override_above_border(Table *table, TableBorderStyle style);
void set_span(Table *table, size_t span_x, size_t span_y);

#ifdef __cplusplus
}
#endif


#pragma once

enum SelectedFilter
{
    NoneFilter, Sub, Up, Average, Paeth
};

//extern SelectedFilter currentSelectedFilter;

//Use None for encoding whole image
enum NumberOfEncodedRows {
    NoneRows, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten
};

extern NumberOfEncodedRows numberOfEncodedRows;
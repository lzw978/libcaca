#include "cucul++.h"

Cucul::Cucul() 
{
    qq = cucul_create(0,0);
    if(!qq) throw -1;
}
Cucul::Cucul(int width, int height) 
{
    qq = cucul_create(width, height);
    if(!qq) throw -1;
}
Cucul::~Cucul() 
{
    if(qq) {
        cucul_free(qq);
    }
}

cucul_t *Cucul::get_cucul_t()
{
    return qq;
}



void Cucul::set_size(unsigned int width, unsigned int height) 
{
    cucul_set_size (qq, width, height);
}
unsigned int Cucul::get_width(void) 
{
    return cucul_get_width (qq);
}
unsigned int Cucul::get_height(void) 
{
    return cucul_get_height (qq);
}
void Cucul::set_color(unsigned int f, unsigned int b) 
{
    cucul_set_color (qq, f, b);
}
char const * Cucul::get_color_name (unsigned int color) 
{
    return cucul_get_color_name (color);
}
void Cucul::putchar (int x, int y, char c)
{
    cucul_putchar (qq, x, y, c);
}
void Cucul::putstr (int x, int y, char *str) 
{
    cucul_putstr(qq, x, y, str);
}
void 	Cucul::printf ( int x , int y , char const * format,...)
{
    char tmp[BUFSIZ];
    char *buf = tmp;
    va_list args;

    va_start(args, format);
#if defined(HAVE_VSNPRINTF)
    vsnprintf(buf, get_width() - x + 1, format, args);
#else
    vsprintf(buf, format, args);
#endif
    buf[get_width() - x] = '\0';
    va_end(args);

    putstr(x, y, buf);

}

void 	Cucul::clear ()
{
    cucul_clear(qq);
}

void 	Cucul::blit ( int x, int y, Cucul* c1, Cucul* c2)
{
    cucul_blit(qq, x, y, c1->get_cucul_t(), c2->get_cucul_t());
}

void 	Cucul::invert ()
{
    cucul_invert(qq);
}

void 	Cucul::flip ()
{
    cucul_flip(qq);
}

void 	Cucul::flop ()
{
    cucul_flop(qq);
}

void 	Cucul::rotate ()
{
    cucul_rotate(qq);
}

void 	Cucul::draw_line (int x1 , int y1, int x2, int y2, char const *c)
{
    cucul_draw_line(qq, x1,y1,x2,y2, c);
}
 
void 	Cucul::draw_polyline (int const x[], int const y[], int f, char const *c)
{
    cucul_draw_polyline(qq, x, y, f, c);
}
 
void 	Cucul::draw_thin_line (int x1 , int y1, int x2, int y2)
{
    cucul_draw_thin_line(qq, x1, y1, x2, y2);
}

void 	Cucul::draw_thin_polyline ( int const x[], int const y[], int f)
{
    cucul_draw_thin_polyline(qq, x, y, f);
}
 
void 	Cucul::draw_circle ( int x, int y, int d, char const *c)
{
    cucul_draw_circle(qq, x, y, d, c);
}

void 	Cucul::draw_ellipse ( int x, int y, int d1, int d2, char const *c)
{
    cucul_draw_ellipse(qq, x, y, d1, d2, c);
}

void 	Cucul::draw_thin_ellipse ( int x, int y, int d1, int d2)
{
    cucul_draw_thin_ellipse(qq, x, y, d1, d2);
}

void 	Cucul::fill_ellipse ( int x, int y, int d1, int d2, char const *c)
{
    cucul_fill_ellipse(qq, x, y, d1, d2, c);
}

void 	Cucul::draw_box ( int x, int y, int w, int h, char const *c)
{
    cucul_draw_box(qq, x, y, w, h, c);
}
 
void 	Cucul::draw_thin_box ( int x, int y, int w, int h)
{
    cucul_draw_thin_box(qq, x, y, w, h);
}

void 	Cucul::fill_box ( int x, int y, int w, int h, char const *c)
{
    cucul_fill_box(qq, x, y, w, h, c);
}

void 	Cucul::draw_triangle ( int x1, int y1, int x2, int y2, int x3, int y3, char const *c)
{
    cucul_draw_triangle(qq, x1, y1, x2, y2, x3, y3, c);
}

void 	Cucul::draw_thin_triangle ( int x1, int y1, int x2, int y2, int x3, int y3)
{
    cucul_draw_thin_triangle(qq, x1, y1, x2, y2, x3, y3);
}

void 	Cucul::fill_triangle ( int x1, int y1, int x2, int y2, int x3, int y3, const char *c)
{
    cucul_fill_triangle(qq, x1, y1, x2, y2, x3, y3, c);
}

int 	Cucul::rand (int min, int max)
{
    return cucul_rand(min, max);
}

unsigned int 	Cucul::sqrt (unsigned int v)
{
    return cucul_sqrt(v);
}

Cucul::Sprite * Cucul::load_sprite (char const *f)
{
    Cucul::Sprite *s = new Cucul::Sprite();
    s->sprite = cucul_load_sprite(f);
    return s;
}

int 	Cucul::get_sprite_frames (Cucul::Sprite const *s)
{
    return cucul_get_sprite_frames(s->sprite);
}

int 	Cucul::get_sprite_width (Cucul::Sprite const *s, int v)
{
    return cucul_get_sprite_width(s->sprite, v);
}

int 	Cucul::get_sprite_height (Cucul::Sprite const *s, int v)
{
    return cucul_get_sprite_height(s->sprite, v);
}

int 	Cucul::get_sprite_dx (Cucul::Sprite const *s, int v)
{
    return cucul_get_sprite_dx(s->sprite, v);
}

int 	Cucul::get_sprite_dy (Cucul::Sprite const *s, int v)
{
    return cucul_get_sprite_dy(s->sprite, v);
}

void 	Cucul::draw_sprite ( int x, int y, Cucul::Sprite const *s, int v)
{
    cucul_draw_sprite(qq, x, y, s->sprite, v);
}

void 	Cucul::free_sprite (Cucul::Sprite *s)
{
    cucul_free_sprite(s->sprite);
}

Cucul::Dither * 	Cucul::create_dither (unsigned int v1, unsigned int v2, unsigned int v3, unsigned int v4, unsigned int v5, unsigned int v6, unsigned int v7, unsigned int v8)
{
    Cucul::Dither *d = new Dither();
    d->dither = cucul_create_dither(v1,v2,v3,v4,v5,v6,v7,v8);
    return d;
}

void 	Cucul::set_dither_palette (Cucul::Dither *d, unsigned int r[], unsigned int g[], unsigned int b[], unsigned int a[])
{
    cucul_set_dither_palette(d->dither, r, g, b, a);
}

void 	Cucul::set_dither_brightness (Cucul::Dither *d, float f)
{
    cucul_set_dither_brightness(d->dither, f);
}

void 	Cucul::set_dither_gamma (Cucul::Dither *d, float f)
{
    cucul_set_dither_gamma(d->dither, f);
}

void 	Cucul::set_dither_contrast ( Cucul::Dither *d, float f)
{
    cucul_set_dither_contrast(d->dither, f);
}

void 	Cucul::set_dither_invert ( Cucul::Dither *d, int i)
{
    cucul_set_dither_invert(d->dither, i);
}

void 	Cucul::set_dither_antialias ( Cucul::Dither *d, char const *c)
{
    cucul_set_dither_antialias(d->dither, c);
}
 
char const *const * 	Cucul::get_dither_antialias_list ( Cucul::Dither const *d)
{
    return cucul_get_dither_antialias_list(d->dither);
}

void 	Cucul::set_dither_color ( Cucul::Dither *d, char const *c)
{
    cucul_set_dither_color(d->dither, c);
}

char const *const * 	Cucul::get_dither_color_list ( Cucul::Dither const *d)
{
    return cucul_get_dither_color_list(d->dither);
}
 
void 	Cucul::set_dither_charset ( Cucul::Dither *d, char const *c)
{
    cucul_set_dither_charset(d->dither, c);
}

char const *const * 	Cucul::get_dither_charset_list ( Cucul::Dither const *d)
{
    return cucul_get_dither_charset_list(d->dither);
}
 
void 	Cucul::set_dither_mode ( Cucul::Dither *d, char const *c)
{
    cucul_set_dither_mode(d->dither, c);
}

char const *const * 	Cucul::get_dither_mode_list ( Cucul::Dither const *d)
{
    return cucul_get_dither_mode_list(d->dither);
}

void 	Cucul::dither_bitmap ( int x, int y, int w, int h,  Cucul::Dither const *d, void *v)
{
    cucul_dither_bitmap(qq, x, y, w, h, d->dither, v);
}

void 	Cucul::free_dither ( Cucul::Dither *d)
{
    cucul_free_dither(d->dither);
}

Cucul::Font * 	Cucul::load_font (void const *s, unsigned int v)
{
    Cucul::Font *f = new Cucul::Font();
    f->font = cucul_load_font(s, v);
    return f;
}

char const *const * 	Cucul::get_font_list (void)
{
    return cucul_get_font_list();
}

unsigned int 	Cucul::get_font_width ( Cucul::Font *f)
{
    return cucul_get_font_width(f->font);
}

unsigned int 	Cucul::get_font_height ( Cucul::Font *f)
{
    return cucul_get_font_height(f->font);
}

void 	Cucul::render_canvas (Cucul::Font *f, unsigned char *c, unsigned int x, unsigned int y, unsigned int w)
{
    cucul_render_canvas(qq, f->font, c, x,y,w);
}

void 	Cucul::free_font ( Cucul::Font *f)
{
    cucul_free_font(f->font);
}

Cucul::Buffer * 	Cucul::create_export (char const *c)
{
    Cucul::Buffer *b = new Cucul::Buffer();
    b->buffer = cucul_create_export(qq, c);
    return b;
}

char const *const * 	Cucul::get_export_list (void)
{
    return cucul_get_export_list();
}

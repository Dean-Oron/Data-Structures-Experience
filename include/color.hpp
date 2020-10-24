/******************************************************************************/
/* 						Shapes oop exercise         						  */
/*						Author - Dean Oron									  */
/*						Date - 14.06.20									      */
/*						Reviewer - Shmuel Pablo							      */
/*						Open Lab 86											  */
/******************************************************************************/

#ifndef ILRD_RD8586_COLOR_HPP
#define ILRD_RD8586_COLOR_HPP
namespace ilrd 
{
class Color
{
public:
    explicit Color(unsigned char red_ = 0, unsigned char green_ = 0, unsigned char blue_ = 0);
    
    inline void SetRed(unsigned char red_); 
    inline void SetGreen(unsigned char green_); 
    inline void SetBlue(unsigned char blue_);
	inline void SetColor(unsigned char r_, unsigned char g_, unsigned char b_);
	
    inline const unsigned char& GetRed() const; 
    inline const unsigned char& GetGreen() const; 
    inline const unsigned char& GetBlue() const;

private:
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
};

inline void Color::SetRed(unsigned char red_)
{
    this->m_red = red_;
}

inline void Color::SetGreen(unsigned char green_)
{
    this->m_green = green_;
}

inline void Color::SetBlue(unsigned char blue_)
{
    this->m_blue = blue_;
}

inline void Color::SetColor(unsigned char r_, unsigned char g_, unsigned char b_)
{
	this->m_red = r_;
	this->m_green = g_;
	this->m_blue = b_;
}

inline const unsigned char& Color::GetRed() const
{
    return (m_red);
}

inline const unsigned char& Color::GetGreen() const
{
    return (m_green);
}

inline const unsigned char& Color::GetBlue() const
{
    return (m_blue);
}

} //ilrd 
#endif //ILRD_RD8586_COLOR_HPP

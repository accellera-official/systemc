class sc_event_and_expr 
{
  public: 
    operator sc_event_and_list const &() const;
    sc_event_and_expr operator & ( sc_event_and_expr, sc_event const& );
};

class sc_event_and_list 
{ 
  public:
    sc_event_and_list(); 
    sc_event_and_list( const sc_event_and_list& ); 
    sc_event_and_list( const sc_event& ); 
    sc_event_and_list& operator= ( const sc_event_and_list& ); 
    ~sc_event_and_list();
    int size() const; 
    void swap( sc_event_and_list& );
    sc_event_and_list& operator&= ( const sc_event& ); 
    sc_event_and_list& operator&= ( const sc_event_and_list& );
    sc_event_and_expr  operator& ( const sc_event& ) const; 
    sc_event_and_expr  operator& ( const sc_event_and_list& ) const;
};

class sc_event_or_expr
{
  public: 
    operator sc_event_or_list const &() const;
    sc_event_or_expr operator | ( sc_event_or_expr† , sc_event const& );
};

class sc_event_or_list 
{ 
  public:
    sc_event_or_list(); 
    sc_event_or_list( const sc_event_or_list& ); 
    sc_event_or_list( const sc_event& ); 
    sc_event_or_list& operator= ( const sc_event_or_list& ); 
    ~sc_event_or_list();
    int size() const; 
    void swap( sc_event_or_list& );
    sc_event_or_list& operator|= ( const sc_event& ); 
    sc_event_or_list& operator|= ( const sc_event_or_list& );
    sc_event_or_expr  operator| ( const sc_event& ) const; 
    sc_event_or_expr  operator| ( const sc_event_or_list& ) const;
};

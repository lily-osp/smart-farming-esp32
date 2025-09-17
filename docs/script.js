// ESP32 Smart Farming Documentation - JavaScript

document.addEventListener('DOMContentLoaded', function() {
    // Initialize all functionality
    initNavigation();
    initScrollEffects();
    initCodeBlocks();
    initInteractiveElements();
    initThemeToggle();
    initSearch();
    initTooltips();
    initLanguageSwitcher();
    initMobileNavigation();
});

// Navigation functionality
function initNavigation() {
    const navLinks = document.querySelectorAll('.nav-link');
    const sections = document.querySelectorAll('section[id]');
    
    // Smooth scrolling for navigation links
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href').substring(1);
            const targetSection = document.getElementById(targetId);
            
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Active navigation highlighting
    window.addEventListener('scroll', function() {
        let current = '';
        sections.forEach(section => {
            const sectionTop = section.offsetTop;
            const sectionHeight = section.clientHeight;
            if (scrollY >= (sectionTop - 200)) {
                current = section.getAttribute('id');
            }
        });
        
        navLinks.forEach(link => {
            link.classList.remove('text-primary', 'bg-gray-100');
            if (link.getAttribute('href') === '#' + current) {
                link.classList.add('text-primary', 'bg-gray-100');
            }
        });
    });
}

// Scroll effects and animations
function initScrollEffects() {
    const observerOptions = {
        threshold: 0.1,
        rootMargin: '0px 0px -50px 0px'
    };
    
    const observer = new IntersectionObserver(function(entries) {
        entries.forEach(entry => {
            if (entry.isIntersecting) {
                entry.target.classList.add('fade-in-up');
            }
        });
    }, observerOptions);
    
    // Observe elements for animation
    const animatedElements = document.querySelectorAll('.bg-gray-50, .bg-white');
    animatedElements.forEach(el => {
        observer.observe(el);
    });
}

// Code block functionality
function initCodeBlocks() {
    const codeBlocks = document.querySelectorAll('pre code');
    
    codeBlocks.forEach(block => {
        // Add copy button
        const copyButton = document.createElement('button');
        copyButton.innerHTML = '<i class="fas fa-copy"></i>';
        copyButton.className = 'absolute top-2 right-2 bg-gray-700 text-white px-2 py-1 rounded text-xs hover:bg-gray-600 transition';
        copyButton.title = 'Copy code';
        
        // Make parent relative for positioning
        const parent = block.parentElement;
        parent.style.position = 'relative';
        parent.appendChild(copyButton);
        
        // Copy functionality
        copyButton.addEventListener('click', function() {
            navigator.clipboard.writeText(block.textContent).then(() => {
                copyButton.innerHTML = '<i class="fas fa-check"></i>';
                copyButton.classList.add('bg-green-600');
                setTimeout(() => {
                    copyButton.innerHTML = '<i class="fas fa-copy"></i>';
                    copyButton.classList.remove('bg-green-600');
                }, 2000);
            });
        });
    });
}

// Interactive elements
function initInteractiveElements() {
    // Feature cards hover effects
    const featureCards = document.querySelectorAll('.bg-gray-50, .bg-white');
    featureCards.forEach(card => {
        card.addEventListener('mouseenter', function() {
            this.classList.add('hover-scale');
        });
        
        card.addEventListener('mouseleave', function() {
            this.classList.remove('hover-scale');
        });
    });
    
    // Button click effects
    const buttons = document.querySelectorAll('button, .btn-primary, .btn-secondary');
    buttons.forEach(button => {
        button.addEventListener('click', function(e) {
            // Create ripple effect
            const ripple = document.createElement('span');
            const rect = this.getBoundingClientRect();
            const size = Math.max(rect.width, rect.height);
            const x = e.clientX - rect.left - size / 2;
            const y = e.clientY - rect.top - size / 2;
            
            ripple.style.width = ripple.style.height = size + 'px';
            ripple.style.left = x + 'px';
            ripple.style.top = y + 'px';
            ripple.classList.add('ripple');
            
            this.appendChild(ripple);
            
            setTimeout(() => {
                ripple.remove();
            }, 600);
        });
    });
}

// Theme toggle functionality
function initThemeToggle() {
    // Check for saved theme preference, system preference, or default to light mode
    const savedTheme = localStorage.getItem('theme');
    const systemPrefersDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
    const currentTheme = savedTheme || (systemPrefersDark ? 'dark' : 'light');
    
    // Set initial theme
    setTheme(currentTheme);

    // Create theme toggle button
    const themeToggle = document.createElement('button');
    themeToggle.id = 'themeToggle';
    themeToggle.innerHTML = currentTheme === 'light' ? '<i class="fas fa-moon"></i>' : '<i class="fas fa-sun"></i>';
    themeToggle.className = 'fixed bottom-4 right-4 bg-primary text-white w-12 h-12 rounded-full shadow-lg hover:bg-secondary transition z-50 focus:outline-none focus:ring-2 focus:ring-primary focus:ring-offset-2';
    themeToggle.setAttribute('aria-label', 'Toggle dark mode');

    document.body.appendChild(themeToggle);

    // Theme toggle functionality
    themeToggle.addEventListener('click', function() {
        const currentTheme = document.documentElement.getAttribute('data-theme');
        const newTheme = currentTheme === 'light' ? 'dark' : 'light';
        
        // Add rotation animation
        this.classList.add('rotating');
        setTimeout(() => {
            this.classList.remove('rotating');
        }, 600);
        
        setTheme(newTheme);
    });

    // Listen for system theme changes
    window.matchMedia('(prefers-color-scheme: dark)').addEventListener('change', function(e) {
        if (!localStorage.getItem('theme')) {
            setTheme(e.matches ? 'dark' : 'light');
        }
    });
}

// Set theme function
function setTheme(theme) {
    document.documentElement.setAttribute('data-theme', theme);
    localStorage.setItem('theme', theme);
    
    // Update theme toggle button icon
    const themeToggle = document.getElementById('themeToggle');
    if (themeToggle) {
        themeToggle.innerHTML = theme === 'light' ? '<i class="fas fa-moon"></i>' : '<i class="fas fa-sun"></i>';
    }
    
    // Update search input styling
    const searchInput = document.getElementById('searchInput');
    if (searchInput) {
        // Force re-application of styles
        searchInput.style.backgroundColor = '';
        searchInput.style.borderColor = '';
        searchInput.style.color = '';
        // Trigger a reflow to ensure styles are applied
        searchInput.offsetHeight;
    }
    
    // Dispatch theme change event
    window.dispatchEvent(new CustomEvent('themeChanged', { 
        detail: { theme: theme } 
    }));
}

// Get current theme
function getCurrentTheme() {
    return document.documentElement.getAttribute('data-theme') || 'light';
}

// Search functionality
function initSearch() {
    // Create search input
    const searchContainer = document.createElement('div');
    searchContainer.className = 'fixed top-20 right-4 z-40';
    searchContainer.innerHTML = `
        <div class="relative">
            <input type="text" id="searchInput" placeholder="Search documentation..." 
                   class="w-64 px-4 py-2 pr-10 border border-gray-300 rounded-lg focus:ring-2 focus:ring-primary focus:border-transparent">
            <i class="fas fa-search absolute right-3 top-3 text-gray-400"></i>
        </div>
        <div id="searchResults" class="absolute top-full left-0 right-0 bg-white border border-gray-300 rounded-lg shadow-lg mt-1 hidden max-h-64 overflow-y-auto"></div>
    `;
    
    document.body.appendChild(searchContainer);
    
    const searchInput = document.getElementById('searchInput');
    const searchResults = document.getElementById('searchResults');
    
    // Search functionality
    searchInput.addEventListener('input', function() {
        const query = this.value.toLowerCase();
        
        if (query.length < 2) {
            searchResults.classList.add('hidden');
            return;
        }
        
        const results = searchContent(query);
        displaySearchResults(results);
    });
    
    // Hide search results when clicking outside
    document.addEventListener('click', function(e) {
        if (!searchContainer.contains(e.target)) {
            searchResults.classList.add('hidden');
        }
    });
}

// Search content function
function searchContent(query) {
    const searchableElements = document.querySelectorAll('h1, h2, h3, h4, h5, h6, p, li, code');
    const results = [];
    
    searchableElements.forEach(element => {
        const text = element.textContent.toLowerCase();
        if (text.includes(query)) {
            const section = element.closest('section');
            const sectionId = section ? section.id : 'overview';
            const sectionTitle = section ? section.querySelector('h2, h3')?.textContent || 'Overview' : 'Overview';
            
            results.push({
                text: element.textContent.substring(0, 100) + '...',
                section: sectionTitle,
                sectionId: sectionId,
                element: element
            });
        }
    });
    
    return results.slice(0, 10); // Limit to 10 results
}

// Display search results
function displaySearchResults(results) {
    const searchResults = document.getElementById('searchResults');
    
    if (results.length === 0) {
        searchResults.innerHTML = '<div class="p-4 text-gray-500 search-no-results">No results found</div>';
    } else {
        searchResults.innerHTML = results.map(result => `
            <div class="p-3 hover:bg-gray-50 cursor-pointer border-b border-gray-100 last:border-b-0 search-result-item" 
                 onclick="scrollToSection('${result.sectionId}')">
                <div class="font-semibold text-sm text-primary search-result-title">${result.section}</div>
                <div class="text-sm text-gray-600 mt-1 search-result-snippet">${result.text}</div>
            </div>
        `).join('');
    }
    
    searchResults.classList.remove('hidden');
}

// Scroll to section function
function scrollToSection(sectionId) {
    const section = document.getElementById(sectionId);
    if (section) {
        section.scrollIntoView({ behavior: 'smooth' });
        document.getElementById('searchResults').classList.add('hidden');
    }
}

// Tooltip functionality
function initTooltips() {
    const tooltipElements = document.querySelectorAll('[title]');
    
    tooltipElements.forEach(element => {
        const title = element.getAttribute('title');
        element.removeAttribute('title');
        
        element.addEventListener('mouseenter', function(e) {
            const tooltip = document.createElement('div');
            tooltip.className = 'absolute bg-gray-900 text-white px-2 py-1 rounded text-sm z-50';
            tooltip.textContent = title;
            tooltip.style.left = e.pageX + 'px';
            tooltip.style.top = (e.pageY - 30) + 'px';
            
            document.body.appendChild(tooltip);
            
            this.tooltip = tooltip;
        });
        
        element.addEventListener('mouseleave', function() {
            if (this.tooltip) {
                this.tooltip.remove();
            }
        });
        
        element.addEventListener('mousemove', function(e) {
            if (this.tooltip) {
                this.tooltip.style.left = e.pageX + 'px';
                this.tooltip.style.top = (e.pageY - 30) + 'px';
            }
        });
    });
}

// Utility functions
function debounce(func, wait) {
    let timeout;
    return function executedFunction(...args) {
        const later = () => {
            clearTimeout(timeout);
            func(...args);
        };
        clearTimeout(timeout);
        timeout = setTimeout(later, wait);
    };
}

function throttle(func, limit) {
    let inThrottle;
    return function() {
        const args = arguments;
        const context = this;
        if (!inThrottle) {
            func.apply(context, args);
            inThrottle = true;
            setTimeout(() => inThrottle = false, limit);
        }
    };
}

// Performance monitoring
function initPerformanceMonitoring() {
    // Monitor page load time
    window.addEventListener('load', function() {
        const loadTime = performance.now();
        console.log(`Page loaded in ${loadTime.toFixed(2)}ms`);
    });
    
    // Monitor scroll performance
    let scrollCount = 0;
    const scrollHandler = throttle(function() {
        scrollCount++;
        if (scrollCount % 100 === 0) {
            console.log(`Scroll events: ${scrollCount}`);
        }
    }, 16);
    
    window.addEventListener('scroll', scrollHandler);
}

// Initialize performance monitoring
initPerformanceMonitoring();

// Error handling
window.addEventListener('error', function(e) {
    console.error('JavaScript error:', e.error);
});

// Service Worker registration (for offline support)
if ('serviceWorker' in navigator) {
    window.addEventListener('load', function() {
        navigator.serviceWorker.register('/sw.js')
            .then(function(registration) {
                console.log('ServiceWorker registration successful');
            })
            .catch(function(err) {
                console.log('ServiceWorker registration failed');
            });
    });
}

// Language Switcher functionality
function initLanguageSwitcher() {
    const langSwitcher = document.getElementById('langSwitcher');
    const langDropdown = document.getElementById('langDropdown');
    const currentLang = document.getElementById('currentLang');
    const langOptions = document.querySelectorAll('.lang-option');
    
    // Check for saved language preference or default to English
    const savedLang = localStorage.getItem('language') || 'en';
    setLanguage(savedLang);
    
    // Toggle dropdown
    langSwitcher.addEventListener('click', function(e) {
        e.stopPropagation();
        const isHidden = langDropdown.classList.contains('hidden');
        if (isHidden) {
            langDropdown.classList.remove('hidden');
            langDropdown.classList.add('show');
        } else {
            langDropdown.classList.add('hidden');
            langDropdown.classList.remove('show');
        }
    });
    
    // Handle language selection
    langOptions.forEach(option => {
        option.addEventListener('click', function() {
            const selectedLang = this.getAttribute('data-lang');
            setLanguage(selectedLang);
            langDropdown.classList.add('hidden');
        });
    });
    
    // Close dropdown when clicking outside
    document.addEventListener('click', function() {
        langDropdown.classList.add('hidden');
        langDropdown.classList.remove('show');
    });
    
    // Prevent dropdown from closing when clicking inside
    langDropdown.addEventListener('click', function(e) {
        e.stopPropagation();
    });
}

// Set language function
function setLanguage(lang) {
    const elements = document.querySelectorAll('[data-en][data-id]');
    const currentLangElement = document.getElementById('currentLang');
    
    // Update current language indicator
    currentLangElement.textContent = lang.toUpperCase();
    
    // Update all translatable elements
    elements.forEach(element => {
        const text = element.getAttribute(`data-${lang}`);
        if (text) {
            element.textContent = text;
        }
    });
    
    // Update page direction for RTL languages if needed
    if (lang === 'ar' || lang === 'he') {
        document.documentElement.setAttribute('dir', 'rtl');
    } else {
        document.documentElement.setAttribute('dir', 'ltr');
    }
    
    // Save language preference
    localStorage.setItem('language', lang);
    
    // Update HTML lang attribute
    document.documentElement.setAttribute('lang', lang);
    
    // Trigger custom event for other components
    window.dispatchEvent(new CustomEvent('languageChanged', { 
        detail: { language: lang } 
    }));
}

// Language-specific utilities
function getCurrentLanguage() {
    return localStorage.getItem('language') || 'en';
}

function translateText(text, lang) {
    // This could be expanded to use a translation service
    // For now, we rely on data attributes
    return text;
}

// Mobile Navigation functionality
function initMobileNavigation() {
    const mobileMenuButton = document.getElementById('mobileMenuButton');
    const mobileMenu = document.getElementById('mobileMenu');
    const menuIcon = document.getElementById('menuIcon');
    const mobileNavLinks = document.querySelectorAll('.mobile-nav-link');
    const mobileLangSwitcher = document.getElementById('mobileLangSwitcher');
    const mobileLangDropdown = document.getElementById('mobileLangDropdown');
    const mobileLangOptions = document.querySelectorAll('.mobile-lang-option');
    const mobileCurrentLang = document.getElementById('mobileCurrentLang');
    
    // Toggle mobile menu
    mobileMenuButton.addEventListener('click', function() {
        const isHidden = mobileMenu.classList.contains('hidden');
        
        if (isHidden) {
            // Show menu
            mobileMenu.classList.remove('hidden');
            menuIcon.classList.add('rotated');
            menuIcon.className = 'fas fa-times text-xl transition-transform duration-300 ease-out';
        } else {
            // Hide menu
            mobileMenu.classList.add('hidden');
            menuIcon.classList.remove('rotated');
            menuIcon.className = 'fas fa-bars text-xl transition-transform duration-300 ease-out';
        }
    });
    
    // Close mobile menu when clicking on a link
    mobileNavLinks.forEach(link => {
        link.addEventListener('click', function() {
            mobileMenu.classList.add('hidden');
            menuIcon.className = 'fas fa-bars text-xl transition-transform duration-300 ease-out';
            menuIcon.classList.remove('rotated');
        });
    });
    
    // Mobile language switcher
    mobileLangSwitcher.addEventListener('click', function(e) {
        e.stopPropagation();
        const isHidden = mobileLangDropdown.classList.contains('hidden');
        if (isHidden) {
            mobileLangDropdown.classList.remove('hidden');
            mobileLangDropdown.classList.add('show');
        } else {
            mobileLangDropdown.classList.add('hidden');
            mobileLangDropdown.classList.remove('show');
        }
    });
    
    // Handle mobile language selection
    mobileLangOptions.forEach(option => {
        option.addEventListener('click', function() {
            const selectedLang = this.getAttribute('data-lang');
            setLanguage(selectedLang);
            mobileLangDropdown.classList.add('hidden');
            mobileLangDropdown.classList.remove('show');
        });
    });
    
    // Close mobile language dropdown when clicking outside
    document.addEventListener('click', function(e) {
        if (!mobileLangSwitcher.contains(e.target) && !mobileLangDropdown.contains(e.target)) {
            mobileLangDropdown.classList.add('hidden');
            mobileLangDropdown.classList.remove('show');
        }
    });
    
    // Update mobile language indicator when language changes
    window.addEventListener('languageChanged', function(e) {
        const lang = e.detail.language;
        mobileCurrentLang.textContent = lang.toUpperCase();
    });
    
    // Close mobile menu when clicking outside
    document.addEventListener('click', function(e) {
        if (!mobileMenuButton.contains(e.target) && !mobileMenu.contains(e.target)) {
            mobileMenu.classList.add('hidden');
            menuIcon.className = 'fas fa-bars text-xl transition-transform duration-300 ease-out';
            menuIcon.classList.remove('rotated');
        }
    });
    
    // Handle window resize
    window.addEventListener('resize', function() {
        if (window.innerWidth >= 768) {
            mobileMenu.classList.add('hidden');
            menuIcon.className = 'fas fa-bars text-xl transition-transform duration-300 ease-out';
            menuIcon.classList.remove('rotated');
        }
    });
    
    // Active navigation highlighting for mobile
    const sections = document.querySelectorAll('section[id]');
    window.addEventListener('scroll', function() {
        let current = '';
        sections.forEach(section => {
            const sectionTop = section.offsetTop;
            const sectionHeight = section.clientHeight;
            if (scrollY >= (sectionTop - 200)) {
                current = section.getAttribute('id');
            }
        });
        
        // Update desktop navigation
        const navLinks = document.querySelectorAll('.nav-link');
        navLinks.forEach(link => {
            link.classList.remove('text-primary', 'bg-gray-100');
            if (link.getAttribute('href') === '#' + current) {
                link.classList.add('text-primary', 'bg-gray-100');
            }
        });
        
        // Update mobile navigation
        mobileNavLinks.forEach(link => {
            link.classList.remove('active');
            if (link.getAttribute('href') === '#' + current) {
                link.classList.add('active');
            }
        });
    });
}

// Export functions for global access
window.scrollToSection = scrollToSection;
window.searchContent = searchContent;
window.displaySearchResults = displaySearchResults;
window.setLanguage = setLanguage;
window.getCurrentLanguage = getCurrentLanguage;
window.setTheme = setTheme;
window.getCurrentTheme = getCurrentTheme;

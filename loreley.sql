-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Dec 06, 2016 at 03:43 PM
-- Server version: 5.7.13-0ubuntu0.16.04.2
-- PHP Version: 7.0.8-0ubuntu0.16.04.3

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `loreley`
--

-- --------------------------------------------------------

--
-- Table structure for table `brands`
--

CREATE TABLE `brands` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `brands`
--

INSERT INTO `brands` (`id`, `name`) VALUES
(4, 'Mertex');

-- --------------------------------------------------------

--
-- Table structure for table `categories`
--

CREATE TABLE `categories` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `categories`
--

INSERT INTO `categories` (`id`, `name`) VALUES
(28, 'Блузки'),
(29, 'Рубашки');

-- --------------------------------------------------------

--
-- Table structure for table `category_sizes`
--

CREATE TABLE `category_sizes` (
  `category_sizes_id` int(11) NOT NULL,
  `category_id` int(11) DEFAULT NULL,
  `size_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- Table structure for table `colors`
--

CREATE TABLE `colors` (
  `id` int(11) NOT NULL,
  `name` tinytext NOT NULL,
  `color` varchar(128) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `colors`
--

INSERT INTO `colors` (`id`, `name`, `color`) VALUES
(4, 'Красный', NULL);

-- --------------------------------------------------------

--
-- Table structure for table `products`
--

CREATE TABLE `products` (
  `id` int(11) NOT NULL,
  `brand` int(11) NOT NULL,
  `category_id` int(11) NOT NULL,
  `characteristic` varchar(256) NOT NULL,
  `color` int(11) NOT NULL,
  `price` decimal(10,0) NOT NULL,
  `note` mediumtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `products`
--

INSERT INTO `products` (`id`, `brand`, `category_id`, `characteristic`, `color`, `price`, `note`) VALUES
(48, 4, 28, 'Джинса', 4, '32100', '312312'),
(49, 4, 28, 'Джинса', 4, '12300', '');

--
-- Triggers `products`
--
DELIMITER $$
CREATE TRIGGER `delete_products_and_subproducts` BEFORE DELETE ON `products` FOR EACH ROW DELETE FROM subproducts WHERE subproducts.product_id = old.id
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `sizes`
--

CREATE TABLE `sizes` (
  `id` int(11) NOT NULL,
  `name` tinytext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `sizes`
--

INSERT INTO `sizes` (`id`, `name`) VALUES
(5, '38'),
(6, '32'),
(7, '36');

-- --------------------------------------------------------

--
-- Table structure for table `subproducts`
--

CREATE TABLE `subproducts` (
  `id` int(11) NOT NULL,
  `product_id` int(11) NOT NULL,
  `size` int(11) NOT NULL,
  `barcode` varchar(13) DEFAULT NULL,
  `note` mediumtext
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subproducts`
--

INSERT INTO `subproducts` (`id`, `product_id`, `size`, `barcode`, `note`) VALUES
(77, 48, 6, '2030000000005', NULL),
(78, 48, 7, '2030000000777', NULL),
(79, 48, 5, '2030000000784', NULL),
(80, 49, 5, '2030000000791', NULL);

--
-- Triggers `subproducts`
--
DELIMITER $$
CREATE TRIGGER `delete_subprod_arrival` BEFORE DELETE ON `subproducts` FOR EACH ROW DELETE FROM subprod_arrival WHERE subprod_arrival.subprod_id = old.id
$$
DELIMITER ;
DELIMITER $$
CREATE TRIGGER `delete_subprod_reduce` BEFORE DELETE ON `subproducts` FOR EACH ROW DELETE FROM subprod_reduce WHERE subprod_reduce.subprod_id = old.id
$$
DELIMITER ;

-- --------------------------------------------------------

--
-- Table structure for table `subprod_arrival`
--

CREATE TABLE `subprod_arrival` (
  `id` int(11) NOT NULL,
  `subprod_id` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `arrival_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subprod_arrival`
--

INSERT INTO `subprod_arrival` (`id`, `subprod_id`, `amount`, `arrival_date`) VALUES
(23, 77, 1, '2016-12-03'),
(24, 78, 1, '2016-12-03'),
(25, 79, 1, '2016-12-03'),
(26, 80, 1, '2016-12-03');

-- --------------------------------------------------------

--
-- Table structure for table `subprod_reasons`
--

CREATE TABLE `subprod_reasons` (
  `id` int(11) NOT NULL,
  `name` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `subprod_reasons`
--

INSERT INTO `subprod_reasons` (`id`, `name`) VALUES
(1, 'Ошибка');

-- --------------------------------------------------------

--
-- Table structure for table `subprod_reduce`
--

CREATE TABLE `subprod_reduce` (
  `id` int(11) NOT NULL,
  `subprod_id` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `reason` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `brands`
--
ALTER TABLE `brands`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `categories`
--
ALTER TABLE `categories`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `category_sizes`
--
ALTER TABLE `category_sizes`
  ADD PRIMARY KEY (`category_sizes_id`);

--
-- Indexes for table `colors`
--
ALTER TABLE `colors`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `products`
--
ALTER TABLE `products`
  ADD PRIMARY KEY (`id`),
  ADD KEY `index_name` (`characteristic`) USING BTREE;

--
-- Indexes for table `sizes`
--
ALTER TABLE `sizes`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subproducts`
--
ALTER TABLE `subproducts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_arrival`
--
ALTER TABLE `subprod_arrival`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_reasons`
--
ALTER TABLE `subprod_reasons`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `subprod_reduce`
--
ALTER TABLE `subprod_reduce`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `brands`
--
ALTER TABLE `brands`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `categories`
--
ALTER TABLE `categories`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=30;
--
-- AUTO_INCREMENT for table `category_sizes`
--
ALTER TABLE `category_sizes`
  MODIFY `category_sizes_id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT for table `colors`
--
ALTER TABLE `colors`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;
--
-- AUTO_INCREMENT for table `products`
--
ALTER TABLE `products`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=50;
--
-- AUTO_INCREMENT for table `sizes`
--
ALTER TABLE `sizes`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT for table `subproducts`
--
ALTER TABLE `subproducts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=81;
--
-- AUTO_INCREMENT for table `subprod_arrival`
--
ALTER TABLE `subprod_arrival`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=27;
--
-- AUTO_INCREMENT for table `subprod_reasons`
--
ALTER TABLE `subprod_reasons`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
--
-- AUTO_INCREMENT for table `subprod_reduce`
--
ALTER TABLE `subprod_reduce`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
